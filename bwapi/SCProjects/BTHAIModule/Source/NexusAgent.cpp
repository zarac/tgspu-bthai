#include "NexusAgent.h"
#include "AgentManager.h"
#include "WorkerAgent.h"
#include "PFManager.h"

NexusAgent::NexusAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("NexusAgent created (%s)", unit->getType().getName().c_str());
	idealNoWorkers = 10;
	level = 1;

	if (analyzed) {
		home = BWTA::getRegion(unit->getTilePosition());
	}

	hasSentWorkers = false;
	if (AgentManager::getInstance()->countNoUnits(UnitTypes::Protoss_Nexus) == 0) {
		//We dont do this for the first Nexus.
		hasSentWorkers = true;
	}
}

void NexusAgent::computeActions() {
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

	if (level == 1) {
		if (AgentManager::getInstance()->countNoUnits(UnitTypes::Protoss_Assimilator) > 0) {
			level = 2;
			idealNoWorkers = 20;
			//Broodwar->printf("[Nexus] Reached level 2");
		}
	}
	if (level == 2) {
		if (AgentManager::getInstance()->countNoUnits(UnitTypes::Protoss_Templar_Archives) > 0) {
			level = 3;
			idealNoWorkers = 30;
			//Broodwar->printf("[Nexus] Reached level 3");
		}
	}

	int noWorkers = AgentManager::getInstance()->getNoWorkers();
	if (noWorkers < idealNoWorkers) {
		UnitType worker = Broodwar->self()->getRace().getWorker();
		if (canBuild(worker)) {
			unit->train(worker);
		}
	}
}

string NexusAgent::getTypeName() {
	return "NexusAgent";
}
