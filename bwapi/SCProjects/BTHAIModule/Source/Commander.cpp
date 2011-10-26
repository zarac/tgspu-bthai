#include "Commander.h"
#include "TerranCommander.h"
#include "ProtossCommander.h"
#include "ZergCommander.h"
#include "AgentManager.h"
#include "ExplorationManager.h"
#include "SpottedObject.h"

Commander* Commander::instance = NULL;

Commander::Commander() {
	currentID = 1;	
	currentState = DEFEND;
	chokepointNo = 0;
}

Commander::~Commander() {
	for (int i = 0; i < (int)squads.size(); i++) {
		delete squads.at(i);
	}
	delete instance;
}

Commander* Commander::getInstance() {
	if (instance == NULL) {
		if (BuildPlanner::isTerran()) {
			instance = new TerranCommander();
		}
		if (BuildPlanner::isProtoss()) {
			instance = new ProtossCommander();
		}
		if (BuildPlanner::isZerg()) {
			instance = new ZergCommander();
		}
	}
	return instance;
}

TilePosition Commander::getClosestEnemyBuilding(TilePosition start) {
	Unit* closestBuilding = NULL;
	SpottedObject* closestObj = NULL;
	double bestDist = -1;

	for(set<Unit*>::const_iterator i=Broodwar->enemy()->getUnits().begin();i!=Broodwar->enemy()->getUnits().end();i++) {
		if ((*i)->getType().isBuilding()) {
			//V2
			double cDist = start.getDistance((*i)->getTilePosition());
			//V1 - TODO: Better, but too damn slow
			//vector<TilePosition> path = BWTA::getShortestPath(start, (*i)->getTilePosition());
			//double cDist = (double)path.size();

			if (!ExplorationManager::canReach(start, (*i)->getTilePosition())) {
				cDist = -1;
			}

			if (bestDist < 0 || cDist < bestDist) {
				bestDist = cDist;
				closestBuilding = (*i);
			}
		}
	}

	if (closestBuilding != NULL) {
		//Broodwar->printf("[TSC]: Closest enemy building is %s", closestBuilding->getType().getName().c_str());
		return closestBuilding->getTilePosition();
	}
	else if (ExplorationManager::getInstance()->isActive()) {
		bestDist = -1;

		vector<SpottedObject*> units = ExplorationManager::getInstance()->getSpottedBuildings();
		for (int i = 0; i < (int)units.size(); i++) {
			SpottedObject* obj = units.at(i);
			if (obj->getType().isBuilding() && obj->isActive()) {
				double cDist = obj->getDistance(start);
				if (bestDist < 0 || cDist < bestDist) {
					bestDist = cDist;
					closestObj = obj;
				}
			}
		}

		//Broodwar->printf("[TSC]: using ExpManager. Closest is %s", closestObj.getType().getName().c_str());
	}
	else {
		//Broodwar->printf("[TSC]: No enemy building found");
		return TilePosition(-1, -1);
	}

	return closestObj->getTilePosition();
}

Squad* Commander::getSquad(int id) {
	for (int i = 0; i < (int)squads.size(); i++) {
		if (squads.at(i)->getID() == id) {
			return squads.at(i);
		}
	}
	return NULL;
}

vector<Squad*> Commander::getSquads() {
	return squads;
}

int Commander::noOffensiveSquadsWithin(TilePosition center, int maxRange) {
	int no = 0;

	for (int i = 0; i < (int)squads.size(); i++) {
		if (squads.at(i)->isActive() && squads.at(i)->isGathered()) {
			TilePosition pos = squads.at(i)->getCenter();
			if (Position(center).getDistance(Position(pos)) <= maxRange) {
				no++;
			}
		}
	}

	return no;
}

void Commander::unitDestroyed(BaseAgent* agent) {
	int squadID = agent->getSquadID();
	if (squadID != -1) {
		Squad* squad = getSquad(squadID);
		if (squad != NULL) {
			squad->removeMember(agent);
		}
		// remove empty squad
		//for ( // I AM HERE
		if (squad->getMembers().size() == 0)
			squad->disband(squad->getCenter());
		//	squads.erase(squads.begin());
	}
}

void Commander::unitCreated(BaseAgent* agent) {
	for (int i = 0; i < (int)squads.size(); i++) {
		if (squads.at(i)->addMember(agent)) {
			break;
		}
	}

	//Check if we can merge to fill a squad
	for (int i = 0; i < (int)squads.size(); i++) {
		if (!squads.at(i)->isFull()) {
			for (int j = 0; j < (int)squads.size(); j++) {
				if (squads.at(i)->canMerge(squads.at(j))) {
					Broodwar->printf("[SQ] Merged Squads %d with %d", squads.at(i)->getID(), squads.at(j)->getID());
				}
			}
		}
	}
}

bool Commander::checkWorkersAttack(BaseAgent *base) {
	bool hasAttacked = false;

	for(set<Unit*>::const_iterator i=Broodwar->enemy()->getUnits().begin();i!=Broodwar->enemy()->getUnits().end();i++) {
		double dist = (*i)->getDistance(base->getUnit());
		if (dist <= 384) {
			//Enemy unit discovered. Attack with all workers.
			AgentManager* agentManager = AgentManager::getInstance();
			for (int j = 0; j < (int)agentManager->size(); j++) {
				BaseAgent* agent = agentManager->at(j);
				if (agent->isAlive() && agent->isWorker()) {
					agent->getUnit()->attackUnit((*i));
					hasAttacked = true;
				}
				AgentManager::release(agent);
			}
		}
	}
	return hasAttacked;
}

int Commander::enemyUnitsWithinRange(TilePosition start, int range) {
	int eCnt = 0;
	for(set<Unit*>::const_iterator i=Broodwar->enemy()->getUnits().begin();i!=Broodwar->enemy()->getUnits().end();i++) {
		double dist = (*i)->getDistance(Position(start));
		if (dist <= range) {
			eCnt++;
		}
		
	}
	return eCnt;
}

void Commander::checkRemovableObstacles() {
	for(set<Unit*>::iterator m = Broodwar->getMinerals().begin(); m != Broodwar->getMinerals().end(); m++) {
		if ((*m)->getResources() <= 20) {
			//Found a mineral that we can remove.
			BaseAgent* baseAgent = AgentManager::getInstance()->getClosestBase((*m)->getTilePosition());
			if (baseAgent != NULL) {
				double cDist = baseAgent->getUnit()->getDistance((*m));
				if (cDist < 1000) {
					//It is close to a base, remove it.

					//Step 1: Check if someone already is working on it
					bool assign = true;
					AgentManager* agentManager = AgentManager::getInstance();
					for (int i = 0; i < (int)agentManager->size(); i++) {
						BaseAgent* agent = agentManager->at(i);
						if (agent->isWorker()) {
							Unit* u = agent->getUnit();
							if (u->isGatheringMinerals()) {
								Unit* t = u->getTarget();
								if (t != NULL && t->getID() == u->getID()) {
									//Someone already working on it. Dont bother.
									assign = false;
								}
							}
						}
						AgentManager::release(agent);
					}

					if (assign) {
						BaseAgent* worker = AgentManager::getInstance()->findClosestFreeWorker((*m)->getTilePosition());
						if (worker != NULL) {
							worker->getUnit()->rightClick((*m));
						}
					}
				}
			}
		}
	}
}

BWTA::Chokepoint* Commander::getNextChokePoint() {
	if (!analyzed) {
		return NULL;
	}

	int cNr = chokepointNo;
	chokepointNo++;
	int noChokes = BaseAgent::getNoChokePoints();
	if (chokepointNo >= noChokes) {
		chokepointNo = 0;
	}

	std::set<BWTA::Chokepoint*> chokepoints= home->getChokepoints();
	BWTA::Chokepoint* choke=NULL;
	int cnt = 0;
	//iterate through all chokepoints and look for the one with the smallest gap (least width)
	for(std::set<BWTA::Chokepoint*>::iterator c=chokepoints.begin();c!=chokepoints.end();c++) {
		if (cnt == cNr) {
			choke=*c;
			break;
		}
		cnt++;
	}
	return choke;
}

bool Commander::needUnit(UnitType type) {
	for (int i = 0; i < (int)squads.size(); i++) {
		if (!squads.at(i)->isFull()) {
			if (squads.at(i)->needUnit(type)) {
				return true;
			}
		}
	}
	return false;
}

bool Commander::checkBaseUnderAttack() {
	bool underAttack = false;

	AgentManager* agentManager = AgentManager::getInstance();
	for (int i = 0; i < (int)agentManager->size(); i++) {
		BaseAgent* agent = agentManager->at(i);
		if (agent->isAlive()) {
			if ((agent->isBuilding() || agent->isWorker()) && agent->isUnderAttack()) {
				//Found building under attack. Can we assist it?
				assistBuilding(agent);
				underAttack = true;
			}
		}
		AgentManager::release(agent);
	}

	return underAttack;
}

void Commander::assistBuilding(BaseAgent* building) {
	for (int i = 0; i < (int)squads.size(); i++) {
		Squad* sq = squads.at(i);
		if (sq->isDefensive()) {
			sq->setGoal(building->getUnit()->getTilePosition());
		}
		if (sq->isOffensive() && currentState == DEFEND) {
			sq->setGoal(building->getUnit()->getTilePosition());
		}
	}
}
