#include "ProtossSquadCommander.h"
#include "AgentManager.h"
#include "ExplorationManager.h"
#include "ComsatAgent.h"

ProtossSquadCommander::ProtossSquadCommander() {
	currentState = STATE_DEFEND;
	currentGoal = TilePosition(-1, -1);
}

ProtossSquadCommander::~ProtossSquadCommander() {
	
}

void ProtossSquadCommander::printInfo() {
	int noHighTemplars = AgentManager::getInstance()->countNoUnits(UnitTypes::Protoss_High_Templar);
	int noDarkTemplars = AgentManager::getInstance()->countNoUnits(UnitTypes::Protoss_Dark_Templar);

	Broodwar->printf("Strength: %d/12000 DarkTemplars: %d, HighTemplars: %d, Goal: (%d,%d)", countOwnStrength(), noDarkTemplars, noHighTemplars, currentGoal.x(), currentGoal.y());
}

void ProtossSquadCommander::computeActions() {
	//Step 1: Check current goal
	if (currentState == STATE_DEFEND) {
		bool attack = false;
		
		int noHighTemplars = AgentManager::getInstance()->countNoUnits(UnitTypes::Protoss_High_Templar);
		int noDarkTemplars = AgentManager::getInstance()->countNoUnits(UnitTypes::Protoss_Dark_Templar);
		if (countOwnStrength() >= 12000 && noHighTemplars >= 2 && noDarkTemplars >= 4) {
			attack = true;
		}

		if (attack) {
			//Attack when we have a large force.
			currentState = STATE_ATTACK;
		}
		else {
			//Check if own buildings is being attacked, then move to defend them
			BWTA::Chokepoint* choke = BaseAgent::getClosestChokePoint();
			if (choke != NULL) {
				currentGoal = TilePosition(choke->getCenter().x() / 32, choke->getCenter().y() / 32);
			}
		}
	}

	//Check if we need workers to attack. Happen if an enemy try to kill off workers before
	//we have attacking units to defend.
	if (currentState == STATE_DEFEND && AgentManager::getInstance()->countNoUnits(UnitTypes::Protoss_Zealot) == 0) {
		BaseAgent* base = AgentManager::getInstance()->getAgent(UnitTypes::Protoss_Nexus);
		if (base != NULL) {
			if (checkWorkersAttack(base)) {
				return;
			}
		}
	}

	//Attack state
	if (currentState == STATE_ATTACK) {
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

	checkImportantUnitsUnderAttack();

	//Step 2: Set goal to agents
	int max = BaseAgent::getNoChokePoints();
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
	}

	if (Broodwar->getFrameCount() % 150 == 0) {
		checkRemovableObstacles();
	}
}

bool ProtossSquadCommander::checkImportantUnitsUnderAttack() {
	AgentManager* agentManager = AgentManager::getInstance();
	for (int i = 0; i < (int)agentManager->size(); i++)  {
		BaseAgent* agent = agentManager->at(i);
		// Double check so we don't get a dead agent. This could cause null pointers.
		if (agent != NULL && agent->isAlive()) {
			// If the agent is damaged it should be repaired. Either if it is a building or an important unit.
			if(isImportantUnit(agent->getUnit())) {
				if (enemyUnitsWithinRange(agent->getUnit()->getTilePosition(), 160) > 0) {
					currentGoal = agent->getUnit()->getTilePosition();
					return true;
				}
			}
		}
		AgentManager::release(agent);
	}
	return false;
}

bool ProtossSquadCommander::isImportantUnit(Unit* unit) {
	UnitType type = unit->getType();

	if (type.isBuilding()) return true;
	if (BaseAgent::isOfType(type, UnitTypes::Protoss_Scout)) return true;
	if (BaseAgent::isOfType(type, UnitTypes::Protoss_Carrier)) return true;
	if (BaseAgent::isOfType(type, UnitTypes::Protoss_Shuttle)) return true;

	return false;
}

TilePosition ProtossSquadCommander::getClosestEnemyBuilding() {
	TilePosition start = Broodwar->self()->getStartLocation();

	//Check if we have any attacking dragoons, if so use them for calculation.
	BaseAgent* agent = AgentManager::getInstance()->getAgent(UnitTypes::Protoss_Dragoon);
	if (agent != NULL) {
		start = agent->getUnit()->getTilePosition();
	}
	
	return getClosestEnemyBuildingTo(start);
}
