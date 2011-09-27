#include "TerranSquadCommander.h"
#include "AgentManager.h"
#include "ExplorationManager.h"
#include "ComsatAgent.h"

TerranSquadCommander::TerranSquadCommander() {
	currentState = STATE_DEFEND;
	currentGoal = TilePosition(-1, -1);
	lastSweepFrame = 0;
}

TerranSquadCommander::~TerranSquadCommander() {
	
}

void TerranSquadCommander::printInfo() {
	string stateStr = "DEFEND";
	if (currentState == STATE_ATTACK) {
		stateStr = "ATTACK";
	}

	int noTanks = AgentManager::getInstance()->countNoUnits(UnitTypes::Terran_Siege_Tank_Tank_Mode) + AgentManager::getInstance()->countNoUnits(UnitTypes::Terran_Siege_Tank_Siege_Mode);
	if (Broodwar->self()->hasResearched(TechTypes::Tank_Siege_Mode)) {
		Broodwar->printf("Strength: %d/5000, NoTanks: %d, SiegeModeResearched: Yes, Goal: (%d,%d) %s", countOwnStrength(), noTanks, currentGoal.x(), currentGoal.y(), stateStr.c_str());
	}
	else {
		Broodwar->printf("Strength: %d/5000, NoTanks: %d, SiegeModeResearched: No, Goal: (%d,%d) %s", countOwnStrength(), noTanks, currentGoal.x(), currentGoal.y(), stateStr.c_str());
	}
}

void TerranSquadCommander::computeActions() {
	//Step 1: Check current goal
	/*if (currentState == STATE_DEFEND) {
		int noTanks = AgentManager::getInstance()->countNoUnits(UnitTypes::Terran_Siege_Tank_Tank_Mode) + AgentManager::getInstance()->countNoUnits(UnitTypes::Terran_Siege_Tank_Siege_Mode);
		if (countOwnStrength() >= 5000 && Broodwar->self()->hasResearched(TechTypes::Tank_Siege_Mode) && noTanks >= 2) {
			//We need siege mode for tanks before we attack enemy base
			currentState = STATE_ATTACK;
		}
		else {
			//Check if own buildings is being attacked, then move to defend them
			BWTA::Chokepoint* choke = BaseAgent::getClosestChokePoint();
			if (choke != NULL) {
				currentGoal = TilePosition(choke->getCenter().x() / 32, choke->getCenter().y() / 32);
			}
		}
	}*/

	//Check if we need workers to attack. Happen if an enemy try to kill off workers before
	//we have attacking units to defend.
	if (currentState == STATE_DEFEND && AgentManager::getInstance()->countNoUnits(UnitTypes::Terran_Marine) == 0) {
		
		BaseAgent* base = AgentManager::getInstance()->getAgent(UnitTypes::Terran_Command_Center);
		if (base != NULL) {
			if (checkWorkersAttack(base)) {
				return;
			}
		}
	}

	//Attack state
	/*if (currentState == STATE_ATTACK) {
		TilePosition closest = getClosestEnemyBuilding();
		if (closest.x() >= 0) {
			currentGoal = closest;
		}
		else {
			BWTA::Chokepoint* choke = BaseAgent::getClosestChokePoint();
			if (choke != NULL) {
				currentGoal = TilePosition(choke->getCenter().x() / 32, choke->getCenter().y() / 32);
			}
		}
		
		if (countOwnStrength() < 1500) {
			currentState = STATE_DEFEND;
			return;
		}

		Broodwar->drawBox(CoordinateType::Map,currentGoal.x()*32,currentGoal.y()*32,currentGoal.x()*32+32,currentGoal.y()*32+32,Colors::Blue,false);
	}

	if (currentState == STATE_ATTACK || currentState == STATE_DEFEND) {
		BaseAgent* agent = AgentManager::getInstance()->getCombatAgent(UnitTypes::Terran_Marine);
		if (agent != NULL) {
			UnitAgent* marine = (UnitAgent*)agent;
			if (marine->isAttacking() || marine->isDefending()) {
				int eCnt = marine->enemyUnitsWithinRange(480);
				if (eCnt > 0) {
					if (Broodwar->getFrameCount() - lastSweepFrame >= 500) {
						//Sweep at closest building
						if (BaseAgent::doScannerSweep(marine->getUnit()->getTilePosition())) {
							lastSweepFrame = Broodwar->getFrameCount();
						}
					}
				}
			}
		}
	}*/

	checkImportantUnitsUnderAttack();

	//Step 2: Set goal to agents
	/*int max = BaseAgent::getNoChokePoints();
	int current = 0;

	AgentManager* agentManager = AgentManager::getInstance();
	for (int i = 0; i < (int)agentManager->size(); i++) {
		BaseAgent* agent = agentManager->at(i);
		if (agent->isAlive()) {
			if (isUnit(agent->getUnit()) && !agent->isExploring()) {
				agent->setGoal(currentGoal);
				if (currentState == STATE_ATTACK) {
					agent->assignToAttack();
				}
				if (currentState == STATE_DEFEND) {
					BWTA::Chokepoint* choke = BaseAgent::getChokePoint(current);
					if (choke != NULL) {
						currentGoal = TilePosition(choke->getCenter().x() / 32, choke->getCenter().y() / 32);
						current++;
						if (current >= max) {
							current = 0;
						}
						agent->setGoal(currentGoal);
					}
					agent->assignToDefend();
				}
			}
		}
		AgentManager::release(agent);
	}*/

	checkUnfinishedBuildings();

	if (Broodwar->getFrameCount() % 150 == 0) {
		checkRemovableObstacles();
	}
}

bool TerranSquadCommander::checkUnfinishedBuildings() {
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

bool TerranSquadCommander::checkImportantUnitsUnderAttack() {
	AgentManager* agentManager = AgentManager::getInstance();
	for (int i = 0; i < (int)agentManager->size(); i++)  {
		BaseAgent* agent = agentManager->at(i);
		// Double check so we don't get a dead agent. This could cause null pointers.
		if (agent != NULL && agent->isAlive()) {
			// If the agent is damaged it should be repaired. Either if it is a building or an important unit.
			if(isImportantUnit(agent->getUnit())) {
				if (enemyUnitsWithinRange(agent->getUnit()->getTilePosition(), 160) > 0) {
					currentGoal = agent->getUnit()->getTilePosition();
				}
				if (!agent->isBuilding() && agent->isDamaged()) {
					repair(agent);
				}

				//If it is a building in our own base, do a scanner sweep
				if (agent->isBuilding() && agent->isUnderAttack()) {
					BaseAgent::doScannerSweep(agent->getUnit()->getTilePosition());
				}
				AgentManager::release(agent);
				return true;
			}
		}
		AgentManager::release(agent);
	}
	return false;
}

bool TerranSquadCommander::isImportantUnit(Unit* unit) {
	UnitType type = unit->getType();

	if (type.isBuilding()) return true;
	if (BaseAgent::isOfType(type, UnitTypes::Terran_Siege_Tank_Tank_Mode)) return true;
	if (BaseAgent::isOfType(type, UnitTypes::Terran_Siege_Tank_Siege_Mode)) return true;
	if (BaseAgent::isOfType(type, UnitTypes::Terran_Science_Vessel)) return true;
	if (BaseAgent::isOfType(type, UnitTypes::Terran_Battlecruiser)) return true;

	return false;
}

void TerranSquadCommander::repair(BaseAgent* agent) {
	//First we must check if someone is repairing this building
	if(AgentManager::getInstance()->isAnyAgentRepairingThisAgent(agent))
		return;

	BaseAgent* repUnit = AgentManager::getInstance()->findClosestFreeWorker(agent->getUnit()->getTilePosition());
	if (repUnit != NULL) {
		repUnit->assignToRepair(agent->getUnit());
	}
}

void TerranSquadCommander::finishBuild(BaseAgent* baseAgent) {
	//First we must check if someone is repairing this building
	if(AgentManager::getInstance()->isAnyAgentRepairingThisAgent(baseAgent))
		return;

	BaseAgent* repUnit = AgentManager::getInstance()->findClosestFreeWorker(baseAgent->getUnit()->getTilePosition());
	if (repUnit != NULL) {
		repUnit->assignToFinishBuild(baseAgent->getUnit());
	}
}

TilePosition TerranSquadCommander::getClosestEnemyBuilding() {
	TilePosition start = Broodwar->self()->getStartLocation();

	//Check if we have any attacking tanks, if so use them for calculation.
	BaseAgent* agent = AgentManager::getInstance()->getAgent(UnitTypes::Terran_Siege_Tank_Siege_Mode);
	if (agent != NULL) {
		start = agent->getUnit()->getTilePosition();
	}

	return getClosestEnemyBuildingTo(start);
}
