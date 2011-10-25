#include "HatcheryAgent.h"
#include "AgentManager.h"
#include "WorkerAgent.h"
#include "PFManager.h"

HatcheryAgent::HatcheryAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("HatcheryAgent created (%s)", unit->getType().getName().c_str());
	idealNoWorkers = 7;
	level = 1;

	if (analyzed) {
		home = BWTA::getRegion(unit->getTilePosition());
	}

	hasSentWorkers = false;
	if (AgentManager::getInstance()->countNoUnits(UnitTypes::Zerg_Hatchery) == 0) {
		//We dont do this for the first Nexus.
		hasSentWorkers = true;
	}
}

void HatcheryAgent::computeActions() {
	UnitType hydralisk = UnitTypes::Zerg_Hydralisk;
	if (Commander::getInstance()->needUnit(hydralisk)) {
		if (canBuild(hydralisk)) {
			unit->train(hydralisk);
			return;
		}
	}

	UnitType zergling = UnitTypes::Zerg_Zergling;
	if (Commander::getInstance()->needUnit(zergling)) {
		if (canBuild(zergling)) {
			unit->train(zergling);
			return;
		}
	}

	if (!hasSentWorkers) {
		if (!unit->isBeingConstructed()) {
			//We have constructed a new base. Make some workers move here.
			int noWorkers = AgentManager::getInstance()->getNoWorkers();
			int toSend = noWorkers / 2;
			int hasSent = 0;

			Broodwar->printf("Sending %d/%d workers to new base", toSend, noWorkers);
			AgentManager* agentManager = AgentManager::getInstance();
			for (int i = 0; i < (int)agentManager->size(); i++) {
				BaseAgent* agent = agentManager->at(i);
				if (agent->isAlive() && agent->isFreeWorker()) {
					Unit* worker = agent->getUnit();
					WorkerAgent* wa = (WorkerAgent*)agent;
					wa->reset();
					worker->rightClick(unit->getPosition());
					//Broodwar->printf("Sending %d to new base", worker->getID());
					hasSent++;
				}
				AgentManager::release(agent);

				if (hasSent >= toSend) {
					break;
				}
			}
			hasSentWorkers = true;
		}
	}

	if (level == 1
		&& AgentManager::getInstance()->countNoUnits(UnitTypes::Zerg_Overlord) > 1)
	{
		level = 2;
		idealNoWorkers = 15;
		Broodwar->printf("[Hatchery] Reached level 2");
	}
	else if (level == 2 && 
		AgentManager::getInstance()->countNoUnits(UnitTypes::Zerg_Extractor) > 0)
	{
		level = 3;
		idealNoWorkers = 20;
		Broodwar->printf("[Hatchery] Reached level 3");
	}
	else if (level == 3
		&& AgentManager::getInstance()->countNoUnits(UnitTypes::Zerg_Hydralisk_Den) > 0)
	{
		level = 4;
		idealNoWorkers = 50;
		Broodwar->printf("[Hatchery] Reached level 4");
	}

	int noWorkers = AgentManager::getInstance()->getNoWorkers();
	if (noWorkers < idealNoWorkers) {
		UnitType worker = Broodwar->self()->getRace().getWorker();
		if (canBuild(worker)) {
			unit->train(worker);
		}
	}
}

string HatcheryAgent::getTypeName() {
	return "HatcheryAgent";
}