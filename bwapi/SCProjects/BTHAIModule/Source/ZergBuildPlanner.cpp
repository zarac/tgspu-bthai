#include "ZergBuildPlanner.h"
#include "WorkerAgent.h"
#include "AgentManager.h"
#include "BaseAgent.h"

ZergBuildPlanner::ZergBuildPlanner() {
	locked = false;
	init();
}

ZergBuildPlanner::~ZergBuildPlanner() {
	
}

void ZergBuildPlanner::init() {
	buildOrder.push_back(UnitTypes::Zerg_Spawning_Pool);
	
	//TODO: This is your buildorder. Add more here.

	level = 1;
}

ZergCommander* ZergBuildPlanner::getCommanderInstance() {
	return (ZergCommander*)Commander::getInstance();
}

void ZergBuildPlanner::computeActions() {
	//NOTE: No need to change this unless some special logic
	//shall be added.

	if (AgentManager::getInstance()->getNoWorkers() == 0) {
		//No workers so cant do anything
		return;
	}

	if (locked) {
		if (Broodwar->getFrameCount() - lockedFrame > getLockedTimer(buildOrder.at(0))) {
			//Locked timeout. Reset it.
			locked = false;
			Broodwar->printf("[BuildPlanner] Locked timeout. Resetting.");
			//Reset the worker to do it
			AgentManager* agentManager = AgentManager::getInstance();
			for (int i = 0; i < (int)agentManager->size(); i++) {
				BaseAgent* agent = agentManager->at(i);
				if (agent->isAlive() && agent->isWorker()) {
					WorkerAgent* worker = (WorkerAgent*)agent;
					if (worker->isConstructing(buildOrder.at(0))) {
						//Reset the worker
						worker->reset();
					}
				}
				AgentManager::release(agent);
			}
		}
		else {
			return;
		}
	}

	//Try to execute first in the list
	if ((int)buildOrder.size() > 0) {
		if (executeOrder(buildOrder.at(0))) {
			locked = true;
			lockedFrame = Broodwar->getFrameCount();
		}
	}

	if (shallBuildOverlord()) {
		// TODO : implement...
	}

	if (mineralsRunningLow()) {
		buildOrder.insert(buildOrder.begin(), UnitTypes::Zerg_Hatchery);
	}
}

void ZergBuildPlanner::unlock(UnitType type) {
	for (vector<UnitType>::iterator it = buildOrder.begin(); it != buildOrder.end(); it++) {
		if (it->getID() == type.getID()) {
			locked = false;
			buildOrder.erase(it);
			return;
		}
	}
}

bool ZergBuildPlanner::shallBuildOverlord() {
	//2. Check if we need supplies
	int supplyTotal = Broodwar->self()->supplyTotal() / 2;
	int supplyUsed = Broodwar->self()->supplyUsed() / 2;
	if (supplyTotal - supplyUsed > 4) {
		return false;
	}

	//3. Check if there is a Pylon already in the list
	if ((int)buildOrder.size() > 0) {
		//if (buildOrder.at(0).getID() == UnitTypes::Zerg_.getID()) {
			//return false;
		//}
	}

	AgentManager *agentManager = AgentManager::getInstance();
	//4. Check if we are already building a Pylon
	for (int i = 0; i < (int)agentManager->size(); i++) {
		BaseAgent* agent = agentManager->at(i);
		if (agent->isAlive()) {
			//if (agent->getUnit()->getType().getID() == UnitTypes::Zerg_Overlord.getID()) {
			//	if (agent->getUnit()->isBeingConstructed()) {
			//		//Found one that is being constructed
			//		AgentManager::release(agent);
			//		return false;
			//	}
			//}
		}
		AgentManager::release(agent);
	}

	Broodwar->printf("Supplies: %d/%d. Adding Pylon to buildorder", supplyUsed, supplyTotal);

	return true;
}

void ZergBuildPlanner::buildingDestroyed(Unit* building) {
	//if (!BaseAgent::isOfType(building, UnitTypes::Zerg_Pylon) && !BaseAgent::isOfType(building, UnitTypes::Zerg_Photon_Cannon)) {
	//	//Non-pylon building destroyed. Rebuild it.
	//	buildOrder.insert(buildOrder.begin(), building->getType());
	//}
}

void ZergBuildPlanner::printInfo() {
	if (locked) {
		if ((int)buildOrder.size() > 0) {
			Broodwar->printf("(PBP) Locked by %s. Timer: %d", buildOrder.at(0).getName().c_str(), Broodwar->getFrameCount() - lockedFrame);
		}
		else {
			Broodwar->printf("(PBP) Locked, but buildorder empty");
		}
	}
	else {
		if ((int)buildOrder.size() > 0) {
			Broodwar->printf("(PBP) Next to build: %s", buildOrder.at(0).getName().c_str());
		}
		else {
			Broodwar->printf("(PBP) Nothing to do");
		}
	}
}