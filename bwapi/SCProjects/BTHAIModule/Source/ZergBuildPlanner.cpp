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
	buildOrder.push_back(UnitTypes::Zerg_Extractor);
	buildOrder.push_back(UnitTypes::Zerg_Hydralisk_Den);
	buildOrder.push_back(UnitTypes::Zerg_Hatchery);
	buildOrder.push_back(UnitTypes::Zerg_Evolution_Chamber);
	buildOrder.push_back(UnitTypes::Zerg_Creep_Colony);
	buildOrder.push_back(UnitTypes::Zerg_Extractor);
	buildOrder.push_back(UnitTypes::Zerg_Evolution_Chamber);
	buildOrder.push_back(UnitTypes::Zerg_Creep_Colony);
	buildOrder.push_back(UnitTypes::Zerg_Hatchery);
	buildOrder.push_back(UnitTypes::Zerg_Creep_Colony);
	buildOrder.push_back(UnitTypes::Zerg_Creep_Colony);
	buildOrder.push_back(UnitTypes::Zerg_Creep_Colony);
	buildOrder.push_back(UnitTypes::Zerg_Creep_Colony);
	level = 1;
}

ZergCommander* ZergBuildPlanner::getCommanderInstance() {
	return (ZergCommander*)Commander::getInstance();
}

void ZergBuildPlanner::computeActions() {
	
	buildOverlordIfNeeded();
	buildHatcheryIfNeeded();

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

	//if (mineralsRunningLow()) {
		//buildOrder.insert(buildOrder.begin(), UnitTypes::Zerg_Hatchery);
	//}
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

/** Build another hatchery, if it seems like a good time */
bool ZergBuildPlanner::buildHatcheryIfNeeded()
{
	// count hatcheries
	std::vector<BaseAgent*> agents = AgentManager::getInstance()->getAgents();
	int hatcheries = 0;
	for (unsigned int i = 0; i < agents.size(); i++)
	{
		BaseAgent *agent = agents.at(i);
		if (agent->isOfType(UnitTypes::Zerg_Hatchery))
		{
			hatcheries++;
		}
	}

	//1. Make sure we have plenty of cache but not so many hatcheries.
	if (Broodwar->self()->minerals() < 300 || hatcheries < 12)
		return false;

	//2. Make sure we're not already building one.
	for (unsigned int i = 0; i < agents.size(); i++)
	{
		BaseAgent *agent = agents.at(i);
		if (agent->isOfType(UnitTypes::Zerg_Hatchery)
			&& !agent->getUnit()->isCompleted())
		{
			return false;
		}
	}

	//Add one to beginning of build-order.
	buildOrder.insert(buildOrder.begin(), UnitTypes::Zerg_Hatchery);

}

/** Build an overlord if needed. Returns true if the case was so.
Should perhaps be the job of ZergCommander? */
bool ZergBuildPlanner::buildOverlordIfNeeded() {
	//Broodwar->printf("minerals : %i , supply: %i,%i", Broodwar->self()->minerals(), Broodwar->self()->supplyUsed(), Broodwar->self()->supplyTotal());

	//1. Make sure we've got minerals.
	if (Broodwar->self()->minerals() < 100)
		return false;

	//2. Check if we need supplies
	int supplyTotal = Broodwar->self()->supplyTotal() / 2;
	int supplyUsed = Broodwar->self()->supplyUsed() / 2;
	if (supplyTotal - supplyUsed > 6 || supplyTotal == 200) {
		//Broodwar->printf("No overlord needed.");
		return false;
	}

	//3. Check if there is a Overlord already being built
	std::vector<BaseAgent*> agents = AgentManager::getInstance()->getAgents();
	for (unsigned int i = 0; i < agents.size(); i++)
	{
		BaseAgent *agent = agents.at(i);
		if (agent->isOfType(UnitTypes::Zerg_Egg))
		{
			//Broodwar->printf("found egg, checking for overlord");
			// TODO : do we need to check for in case an overlord is being morphed?
			if (agent->getUnit()->getBuildType() == UnitTypes::Zerg_Overlord)
			{
				//Broodwar->printf("already training overlord");
				return false;
			}
		}
	}

	//Broodwar->printf("Supplies: %d/%d. Building Overlord. Frame count: %d", supplyUsed, supplyTotal, Broodwar->getFrameCount());

	//std::vector<BaseAgent*> agents = AgentManager::getInstance()->getAgents();
	for (unsigned int i = 0; i < agents.size(); i++)
	{
		BaseAgent *agent = agents.at(i);
		if (agent->isOfType(UnitTypes::Zerg_Larva))
		{
			//Broodwar->printf("unit is larva, training overlord");
			agent->getUnit()->train(UnitTypes::Zerg_Overlord);
			return true;
		}
	}
	//Broodwar->printf("No larva for available, not training.");
	return false;
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