#include "TerranCommander.h"
#include "AgentManager.h"
#include "ExplorationManager.h"

TerranCommander::TerranCommander() {
	currentID = 1;	
	currentState = DEFEND;

	addMainAttackSquad();
	addBunkerSquad();
	
	//TODO: Add more squads here.
	
}

void TerranCommander::addBunkerSquad() {
	//NOTE: Bunkersquads shall always contain 4 units
	Squad* squad = new Squad(currentID, Squad::BUNKER, "BunkerDefenceSquad");
	squad->addSetup(UnitTypes::Terran_Marine, 2);
	squad->addSetup(UnitTypes::Terran_Firebat, 2);
	squads.push_back(squad);
	currentID++;
}

void TerranCommander::addMainAttackSquad() {
	Squad* squad = new Squad(currentID, Squad::OFFENSIVE, "MainAttackSquad");
	squad->addSetup(UnitTypes::Terran_Marine, 4);
	squad->addSetup(UnitTypes::Terran_Firebat, 2);
	squad->addSetup(UnitTypes::Terran_Medic, 2);
	squad->addSetup(UnitTypes::Terran_Vulture, 1);
	squad->addSetup(UnitTypes::Terran_Siege_Tank_Tank_Mode, 1);
	squads.push_back(squad);
	currentID++;
}

TerranCommander::~TerranCommander() {
	for (int i = 0; i < (int)squads.size(); i++) {
		delete squads.at(i);
	}
	delete instance;
}

void TerranCommander::computeActions() {
	if (currentState == DEFEND) {
		int noOffSquads = 0;

		for (int i = 0; i < (int)squads.size(); i++) {
			if (squads.at(i)->isFull() && squads.at(i)->isOffensive()) {
				noOffSquads++;
			}
		}

		//Time to attack
		//TODO: Currently attacks when we have 2 full squads. Change if needed.
		if (noOffSquads >= 2) {
			currentState = ATTACK;
		}
	}

	if (currentState == DEFEND) {
		for (int i = 0; i < (int)squads.size(); i++) {
			//Scanner Sweep if we are under attack
			if (squads.at(i)->isUnderAttack()) {
				BaseAgent::doScannerSweep(squads.at(i)->getCenter());
			}

			if (!squads.at(i)->hasGoal()) {
				BWTA::Chokepoint* choke = getNextChokePoint();
				if (choke != NULL) {
					//Broodwar->printf("Assigned SQ %d to chokepoint %d", squads.at(i)->getID(), chokepointNo);
					TilePosition defSpot = TilePosition(choke->getCenter());
					squads.at(i)->setGoal(defSpot);
				}
			}
		}
	}

	if (currentState == ATTACK) {
		for (int i = 0; i < (int)squads.size(); i++) {
			//Scanner Sweep if we are under attack
			if (squads.at(i)->isUnderAttack()) {
				BaseAgent::doScannerSweep(squads.at(i)->getCenter());
			}

			if (squads.at(i)->isActive() && squads.at(i)->isOffensive()) {
				TilePosition closest = getClosestEnemyBuilding(Broodwar->self()->getStartLocation());
				if (closest.x() >= 0) {
					squads.at(i)->setGoal(closest);
				}
			}
			else {
				BWTA::Chokepoint* choke = getNextChokePoint();
				if (choke != NULL) {
					TilePosition defSpot = TilePosition(choke->getCenter());
					squads.at(i)->setGoal(defSpot);
				}
			}
		}
	}

	//Check if we need workers to attack. Happen if an enemy try to kill off workers before
	//we have attacking units to defend.
	if (currentState == DEFEND) {// && AgentManager::getInstance()->countNoUnits(UnitTypes::Terran_Marine) == 0) {	
		BaseAgent* base = AgentManager::getInstance()->getAgent(UnitTypes::Terran_Command_Center);
		if (base != NULL) {
			if (checkWorkersAttack(base)) {
				return;
			}
		}
	}

	//Check if there are obstacles we can remove.
	if (Broodwar->getFrameCount() % 150 == 0) {
		checkRemovableObstacles();
	}

	//Check if there are unfinished buildings we need
	//to complete.
	checkUnfinishedBuildings();

	//Check if there are units we need to repair.
	checkRepairUnits();

	//Check if base is under attack. If so, assist it
	checkBaseUnderAttack();
}

bool TerranCommander::checkRepairUnits() {
	AgentManager* agentManager = AgentManager::getInstance();
	for (int i = 0; i < (int)agentManager->size(); i++) 
	{
		BaseAgent* agent = agentManager->at(i);
		if (agent != NULL && agent->isAlive() && isImportantUnit(agent->getUnit()) && agent->isDamaged() && !agent->getUnit()->isBeingConstructed())
		{
			repair(agent);
		}
		AgentManager::release(agent);
	}
	return false;
}

bool TerranCommander::isImportantUnit(Unit* unit) {
	UnitType type = unit->getType();

	if (type.isBuilding()) return true;
	if (BaseAgent::isOfType(type, UnitTypes::Terran_Siege_Tank_Tank_Mode)) return true;
	if (BaseAgent::isOfType(type, UnitTypes::Terran_Siege_Tank_Siege_Mode)) return true;
	if (BaseAgent::isOfType(type, UnitTypes::Terran_Science_Vessel)) return true;
	if (BaseAgent::isOfType(type, UnitTypes::Terran_Battlecruiser)) return true;

	return false;
}

void TerranCommander::repair(BaseAgent* agent) {
	//First we must check if someone is repairing this building
	if(AgentManager::getInstance()->isAnyAgentRepairingThisAgent(agent))
		return;

	BaseAgent* repUnit = AgentManager::getInstance()->findClosestFreeWorker(agent->getUnit()->getTilePosition());
	if (repUnit != NULL) {
		Broodwar->printf("Assigned SCV %d to repair %s", repUnit->getUnitID(), agent->getUnit()->getType().getName().c_str());
		repUnit->assignToRepair(agent->getUnit());
	}
}

void TerranCommander::finishBuild(BaseAgent* baseAgent) {
	//First we must check if someone is repairing this building
	if(AgentManager::getInstance()->isAnyAgentRepairingThisAgent(baseAgent))
		return;

	BaseAgent* repUnit = AgentManager::getInstance()->findClosestFreeWorker(baseAgent->getUnit()->getTilePosition());
	if (repUnit != NULL) {
		repUnit->assignToFinishBuild(baseAgent->getUnit());
	}
}

bool TerranCommander::checkUnfinishedBuildings() {
	AgentManager* agentManager = AgentManager::getInstance();
	for (int i = 0; i < (int)agentManager->size(); i++) 
	{
		BaseAgent* agent = agentManager->at(i);
		// Double check so we don't get a dead agent. This could cause null pointers.
		if (agent != NULL && agent->isAlive() && agent->isBuilding() && agent->isDamaged())
		{
			Unit* builder = agent->getUnit()->getBuildUnit();
			if (builder == NULL || !builder->isConstructing()) {
				finishBuild(agent);
			}
		}
		AgentManager::release(agent);
	}
	return false;
}
