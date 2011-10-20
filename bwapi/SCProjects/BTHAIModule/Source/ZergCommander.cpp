#include "ZergCommander.h"
#include "AgentManager.h"
#include "ExplorationManager.h"

ZergCommander::ZergCommander() {
	currentID = 1;	
	currentState = DEFEND;
	addMainAttackSquad();
}

void ZergCommander::addMainAttackSquad() {
	Squad* squad = new Squad(currentID, Squad::OFFENSIVE, "MainAttackSquad");
	squad->addSetup(UnitTypes::Zerg_Zergling, 4);
	squads.push_back(squad);
	currentID++;
}

ZergCommander::~ZergCommander() {
	for (int i = 0; i < (int)squads.size(); i++) {
		delete squads.at(i);
	}
	delete instance;
}

void ZergCommander::computeActions() {
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
}