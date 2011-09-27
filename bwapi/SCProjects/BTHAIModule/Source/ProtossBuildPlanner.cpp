#include "ProtossBuildPlanner.h"
#include "WorkerAgent.h"
#include "AgentManager.h"
#include "BaseAgent.h"

ProtossBuildPlanner::ProtossBuildPlanner() {
	locked = false;
	init();
}

ProtossBuildPlanner::~ProtossBuildPlanner() {
	
}

void ProtossBuildPlanner::init() {
	buildOrder.push_back(UnitTypes::Protoss_Pylon);
	buildOrder.push_back(UnitTypes::Protoss_Pylon);
	buildOrder.push_back(UnitTypes::Protoss_Gateway);
	buildOrder.push_back(UnitTypes::Protoss_Pylon);
	buildOrder.push_back(UnitTypes::Protoss_Assimilator);
	
	//TODO: This is your buildorder. Add more here.

	level = 1;
}

ProtossCommander* ProtossBuildPlanner::getCommanderInstance() {
	return (ProtossCommander*)Commander::getInstance();
}

void ProtossBuildPlanner::computeActions() {
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

	if (shallBuildPylon()) {
		buildOrder.insert(buildOrder.begin(), UnitTypes::Protoss_Pylon);
		buildOrder.insert(buildOrder.begin(), UnitTypes::Protoss_Pylon);
	}

	if (mineralsRunningLow()) {
		buildOrder.insert(buildOrder.begin(), UnitTypes::Protoss_Nexus);
	}
}

void ProtossBuildPlanner::unlock(UnitType type) {
	UnitType pc = UnitTypes::Protoss_Photon_Cannon;
	if (type.getID() == pc.getID()) {
		int noChokes = BaseAgent::getNoChokePoints();
		if (noChokes > AgentManager::getInstance()->countNoUnits(pc)) {
			//Added another photon cannon
			buildOrder.insert(buildOrder.begin(), pc);
		}
	}
	for (vector<UnitType>::iterator it = buildOrder.begin(); it != buildOrder.end(); it++) {
		if (it->getID() == type.getID()) {
			locked = false;
			buildOrder.erase(it);
			return;
		}
	}
}

bool ProtossBuildPlanner::shallBuildPylon() {
	//1. Check if any building is unpowered
	AgentManager* agentManager = AgentManager::getInstance();
	for (int i = 0; i < (int)agentManager->size(); i++) {
		BaseAgent* agent = agentManager->at(i);
		if (agent->isAlive()) {
			Unit* cUnit = agent->getUnit();
			if (cUnit->isUnpowered()) {
				AgentManager::release(agent);
				return true;
			}
		}
		AgentManager::release(agent);
	}

	//2. Check if we need supplies
	int supplyTotal = Broodwar->self()->supplyTotal() / 2;
	int supplyUsed = Broodwar->self()->supplyUsed() / 2;
	if (supplyTotal - supplyUsed > 4) {
		return false;
	}

	//3. Check if there is a Pylon already in the list
	if ((int)buildOrder.size() > 0) {
		if (buildOrder.at(0).getID() == UnitTypes::Protoss_Pylon.getID()) {
			return false;
		}
	}

	//4. Check if we are already building a Pylon
	for (int i = 0; i < (int)agentManager->size(); i++) {
		BaseAgent* agent = agentManager->at(i);
		if (agent->isAlive()) {
			if (agent->getUnit()->getType().getID() == UnitTypes::Protoss_Pylon.getID()) {
				if (agent->getUnit()->isBeingConstructed()) {
					//Found one that is being constructed
					AgentManager::release(agent);
					return false;
				}
			}
		}
		AgentManager::release(agent);
	}

	Broodwar->printf("Supplies: %d/%d. Adding Pylon to buildorder", supplyUsed, supplyTotal);

	return true;
}

void ProtossBuildPlanner::buildingDestroyed(Unit* building) {
	if (!BaseAgent::isOfType(building, UnitTypes::Protoss_Pylon) && !BaseAgent::isOfType(building, UnitTypes::Protoss_Photon_Cannon)) {
		//Non-pylon building destroyed. Rebuild it.
		buildOrder.insert(buildOrder.begin(), building->getType());
	}
}

int ProtossBuildPlanner::getPylonAtChokePointNo() {
	return pylonAtChokePointNo;
}

void ProtossBuildPlanner::increasePylonAtChokePointNo() {
	pylonAtChokePointNo++;
}

int ProtossBuildPlanner::getShieldBatteryAtChokePointNo() {
	return shieldBatteryAtChokePointNo;
}

void ProtossBuildPlanner::increaseShieldBatteryAtChokePointNo() {
	shieldBatteryAtChokePointNo++;
}

void ProtossBuildPlanner::printInfo() {
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