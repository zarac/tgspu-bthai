#include "TerranBuildPlanner.h"
#include "WorkerAgent.h"
#include "AgentManager.h"
#include "BaseAgent.h"
#include "ExplorationManager.h"
#include "Commander.h"
#include "TerranCommander.h"

TerranBuildPlanner::TerranBuildPlanner() {
	locked = false;
	init();
}

TerranBuildPlanner::~TerranBuildPlanner() {
	
}

void TerranBuildPlanner::init() {
	buildOrder.push_back(UnitTypes::Terran_Barracks);
	buildOrder.push_back(UnitTypes::Terran_Refinery);
	buildOrder.push_back(UnitTypes::Terran_Bunker);
	
	//TODO: This is your buildorder. Add more here.
}

TerranCommander* TerranBuildPlanner::getCommanderInstance() {
	return (TerranCommander*)Commander::getInstance();
}

void TerranBuildPlanner::computeActions() {
	//NOTE: No need to change anything here unless a special
	//logic shall be added.

	if (AgentManager::getInstance()->getNoWorkers() == 0) {
		//No workers so cant do anything
		return;
	}

	if (locked) {
		if (Broodwar->getFrameCount() - lockedFrame > getLockedTimer(buildOrder.at(0))) {
			//Locked timeout. Reset it.
			locked = false;
			Broodwar->printf("[BuildPlanner] Locked timeout for %s. Resetting.", buildOrder.at(0).getName().c_str());
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
			if (!buildOrder.at(0).isAddon()) {
				locked = true;
				lockedFrame = Broodwar->getFrameCount();
			}
		}
	}

	if (shallBuildSupplyDepot()) {
		buildOrder.insert(buildOrder.begin(), UnitTypes::Terran_Supply_Depot);
		buildOrder.insert(buildOrder.begin(), UnitTypes::Terran_Supply_Depot);
	}

	if (mineralsRunningLow()) {
		buildOrder.insert(buildOrder.begin(), UnitTypes::Terran_Command_Center);
	}
}

void TerranBuildPlanner::unlock(UnitType type) {
	UnitType b = UnitTypes::Terran_Bunker;
	if (BaseAgent::isOfType(type, b)) {
		int noChokes = BaseAgent::getNoChokePoints();
		if (noChokes > AgentManager::getInstance()->countNoUnits(b)) {
			//Added another Bunker
			buildOrder.insert(buildOrder.begin(), b);
		}
	}
	for (vector<UnitType>::iterator it = buildOrder.begin(); it != buildOrder.end(); it++) {
		if (BaseAgent::isOfType((*it), type)) {
			locked = false;
			buildOrder.erase(it);
			return;
		}
	}
}

bool TerranBuildPlanner::shallBuildSupplyDepot() {
	//1. Check if we need supplies
	int supplyTotal = Broodwar->self()->supplyTotal() / 2;
	int supplyUsed = Broodwar->self()->supplyUsed() / 2;
	if (supplyTotal - supplyUsed > 4) {
		return false;
	}

	//2. Check if there is a SupplyDepot already in the list
	if ((int)buildOrder.size() > 0) {
		if (BaseAgent::isOfType(buildOrder.at(0), UnitTypes::Terran_Supply_Depot)) {
			return false;
		}
	}

	//3. Check if we are already building a SupplyDepot
	AgentManager* agentManager = AgentManager::getInstance();
	for (int i = 0; i < (int)agentManager->size(); i++) {
		BaseAgent* agent = agentManager->at(i);
		if (agent->isAlive()) {
			if (agent->isOfType(UnitTypes::Terran_Supply_Depot)) {
				if (agent->getUnit()->isBeingConstructed()) {
					//Found one that is being constructed
					AgentManager::release(agent);
					return false;
				}
			}
		}
		AgentManager::release(agent);
	}

	Broodwar->printf("Supply Depot needed %d/%d", supplyUsed, supplyTotal);

	return true;
}

void TerranBuildPlanner::buildingDestroyed(Unit* building) {
	if (!BaseAgent::isOfType(building, UnitTypes::Terran_Supply_Depot)) {
		//Not-supply depot building destroyed
		buildOrder.insert(buildOrder.begin(), building->getType());
	}
}

void TerranBuildPlanner::printInfo() {
	if (locked) {
		if ((int)buildOrder.size() > 0) {
			Broodwar->printf("(TBP) Locked by %s. Timer: %d", buildOrder.at(0).getName().c_str(), Broodwar->getFrameCount() - lockedFrame);
		}
		else {
			Broodwar->printf("(TBP) Locked, but buildorder empty");
		}
	}
	else {
		if ((int)buildOrder.size() > 0) {
			Broodwar->printf("(TBP) Next to build: %s", buildOrder.at(0).getName().c_str());
		}
		else {
			Broodwar->printf("(TBP) Nothing to do");
		}
	}
}
