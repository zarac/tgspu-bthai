#include "BuildPlanner.h"
#include "WorkerAgent.h"
#include "BaseAgent.h"
#include "AgentManager.h"
#include "TerranBuildPlanner.h"
#include "ProtossBuildPlanner.h"
#include "ZergBuildPlanner.h"
#include "ExplorationManager.h"

BuildPlanner* BuildPlanner::instance = NULL;

BuildPlanner::BuildPlanner() {
	locked = false;
	init();
	chokepointNo = 0;
	pylonAtChokePointNo = 0;
	shieldBatteryAtChokePointNo = 0;
}

BuildPlanner::~BuildPlanner() {
	delete instance;
}

BuildPlanner* BuildPlanner::getInstance() {
	if (instance == NULL) {
		if (isTerran()) {
			instance = new TerranBuildPlanner();
		}
		if (isProtoss()) {
			instance = new ProtossBuildPlanner();
		}
		if (isZerg()) {
			instance = new ZergBuildPlanner();
		}
	}
	return instance;
}

bool BuildPlanner::isLocked() {
	if (locked) {
		if (Broodwar->self()->minerals() >= 500 && Broodwar->self()->gas() >= 300) {
			//We have plenty of resources. Dont bother about lock.
			return false;
		}
		else {
			return true;
		}
	}
	return false;
}

void BuildPlanner::unlock(UnitType type) {
	for (vector<UnitType>::iterator it = buildOrder.begin(); it != buildOrder.end(); it++) {
		if (it->getID() == type.getID()) {
			locked = false;
			buildOrder.erase(it);
			return;
		}
	}
}

bool BuildPlanner::executeOrder(UnitType type) {
	AgentManager* agentManager = AgentManager::getInstance();
	for (int i = 0; i < (int)agentManager->size(); i++) {
		BaseAgent* agent = agentManager->at(i);
		if (agent != NULL && agent->isAlive()) {
			if (agent->canBuild(type)) {
				agent->assignToBuild(type);
				AgentManager::release(agent);
				return true;
			}
		}
		AgentManager::release(agent);
	}
	return false;
}

int BuildPlanner::getChokepointNo() {
	int c = chokepointNo;

	chokepointNo++;
	int noChokes = BaseAgent::getNoChokePoints();
	if (chokepointNo >= noChokes) {
		chokepointNo = 0;
	}

	return c;
}

bool BuildPlanner::isTerran() {
	if (Broodwar->self()->getRace().getID() == Races::Terran.getID()) {
		return true;
	}
	return false;
}

bool BuildPlanner::isProtoss() {
	if (Broodwar->self()->getRace().getID() == Races::Protoss.getID()) {
		return true;
	}
	return false;
}

bool BuildPlanner::isZerg() {
	if (Broodwar->self()->getRace().getID() == Races::Zerg.getID()) {
		return true;
	}
	return false;
}

int BuildPlanner::getLockedTimer(UnitType type) {
	int t = 400;
	if (type.isResourceDepot()) {
		t = 1500;
	}
	if (type.isRefinery()) {
		t = 1000;
	}
	if (BaseAgent::isOfType(type, UnitTypes::Terran_Bunker)) {
		t = 900;
	}
	if (BaseAgent::isOfType(type, UnitTypes::Terran_Missile_Turret)) {
		t = 900;
	}
	if (BaseAgent::isOfType(type, UnitTypes::Protoss_Photon_Cannon)) {
		t = 900;
	}
	return t;
}

bool BuildPlanner::mineralsRunningLow() {
	UnitType baseType = UnitTypes::Terran_Command_Center;
	if (isProtoss()) {
		baseType = UnitTypes::Protoss_Nexus;
	}
	else if (isZerg()) {
		baseType = UnitTypes::Zerg_Hatchery;
	}


	int mineralLimit = 1200; //TODO: Is this a good limit?

	int baseCnt = 0;
	int lowCnt = 0;
	AgentManager* agentManager = AgentManager::getInstance();
	for (int i = 0; i < (int)agentManager->size(); i++) {
		BaseAgent* agent = agentManager->at(i);
		if (agent->isAlive() && agent->isOfType(baseType)) {
			baseCnt++;
			int totMinerals = ExplorationManager::getInstance()->calcMineralsAroundPosition(agent->getUnit()->getTilePosition());
			if (totMinerals <= mineralLimit) {
				lowCnt++;
			}
		}
		AgentManager::release(agent);
	}

	bool low = false;
	if (lowCnt >= baseCnt) {
		low = true;
	}

	if (low) {
		//1. Check if there is a base already in the list
		for (int i = 0; i < (int)buildOrder.size(); i++) {
			if (baseType.getID() == buildOrder.at(i).getID()) {
				return false;
			}
		}

		//2. Check if we are already building a base
		AgentManager* agentManager = AgentManager::getInstance();
		for (int i = 0; i < (int)agentManager->size(); i++) {
			BaseAgent* agent = agentManager->at(i);
			if (agent->isAlive()) {
				if (agent->isOfType(baseType)) {
					if (agent->getUnit()->isBeingConstructed()) {
						//Found one that is being constructed
						AgentManager::release(agent);
						return false;
					}
				}
			}
			AgentManager::release(agent);
		}
	}

	return low;
}
