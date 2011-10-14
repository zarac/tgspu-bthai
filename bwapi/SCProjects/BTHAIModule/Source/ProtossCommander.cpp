#include "ProtossCommander.h"
#include "AgentManager.h"
#include "ExplorationManager.h"

ProtossCommander::ProtossCommander() {
	currentID = 1;	
	currentState = DEFEND;

	addMainAttackSquad();
	addDragoonSquad();
	
	//TODO: Add more squads here.
}

ProtossCommander::~ProtossCommander() {
	for (int i = 0; i < (int)squads.size(); i++) {
		delete squads.at(i);
	}
	delete instance;
}


void ProtossCommander::addDragoonSquad() {
	Squad* squad = new Squad(currentID, Squad::OFFENSIVE, "DragoonSquad");
	squad->addSetup(UnitTypes::Protoss_Dragoon, 12);
	squads.push_back(squad);
	currentID++;
}

void ProtossCommander::addMainAttackSquad() {
	Squad* squad = new Squad(currentID, Squad::OFFENSIVE, "MainAttackSquad");
	squad->addSetup(UnitTypes::Protoss_Zealot, 6);
	squad->addSetup(UnitTypes::Protoss_Dragoon, 2);
	squads.push_back(squad);
	currentID++;
}

void ProtossCommander::computeActions() {
	if (currentState == DEFEND) {
		int noOffSquads = 0;

		for (int i = 0; i < (int)squads.size(); i++) {
			if (squads.at(i)->isFull() && squads.at(i)->isOffensive()) {
				noOffSquads++;
			}
		}

		//Time to attack
		if (noOffSquads >= 2) { //NOTE: Attacks when we have 2 full squads. Change if necessary.
			currentState = ATTACK;
		}
	}

	if (currentState == DEFEND) {
		BWTA::Chokepoint* choke = BaseAgent::getClosestChokePoint();
		if (choke != NULL) {
			for (int i = 0; i < (int)squads.size(); i++) {
				if (!squads.at(i)->hasGoal()) {
					TilePosition defSpot = TilePosition(choke->getCenter());
					squads.at(i)->setGoal(defSpot);
				}
			}
		}
	}

	if (currentState == ATTACK) {
		for (int i = 0; i < (int)squads.size(); i++) {
			if (squads.at(i)->isActive() && squads.at(i)->isOffensive()) {
				TilePosition closest = getClosestEnemyBuilding(Broodwar->self()->getStartLocation());
				if (closest.x() >= 0) {
					squads.at(i)->setGoal(closest);
				}
			}
			else {
				BWTA::Chokepoint* choke = BaseAgent::getClosestChokePoint();
				if (choke != NULL) {
					TilePosition defSpot = TilePosition(choke->getCenter());
					squads.at(i)->setGoal(defSpot);
				}
			}
		}
	}

	//Check if we need workers to attack. Happen if an enemy try to kill off workers before
	//we have attacking units to defend.
	if (currentState == DEFEND && AgentManager::getInstance()->countNoUnits(UnitTypes::Protoss_Zealot) == 0) {
		
		BaseAgent* base = AgentManager::getInstance()->getAgent(UnitTypes::Protoss_Nexus);
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

	//Check if base is under attack. If so, assist it
	checkBaseUnderAttack();
}
