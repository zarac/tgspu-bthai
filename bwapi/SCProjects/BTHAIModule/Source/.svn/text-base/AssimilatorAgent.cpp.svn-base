#include "AssimilatorAgent.h"
#include "AgentManager.h"
#include "WorkerAgent.h"

AssimilatorAgent::AssimilatorAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("AssimilatorAgent created (%s)", unit->getType().getName().c_str());
}

void AssimilatorAgent::computeActions() {
	if (Broodwar->getFrameCount() % 100 == 0) {
		for (int i = 0; i < (int)assignedWorkers.size(); i++) {
			if (!assignedWorkers.at(i)->isAlive()) {
				assignedWorkers.erase(assignedWorkers.begin() + i);
				i--;
			}
			else if (assignedWorkers.at(i)->getUnit()->isGatheringMinerals()) {
				//The workers have reset. Bring them back to the refinery.
				assignedWorkers.at(i)->getUnit()->rightClick(unit);
			}
		}
	}

	if ((int)assignedWorkers.size() <= 3) {
		if (!unit->isBeingConstructed()) {
			WorkerAgent* worker = (WorkerAgent*)AgentManager::getInstance()->getFreeWorker();
			if (worker != NULL)
			{
				worker->UnLockAgent();
				worker->getUnit()->rightClick(unit);
				worker->setState(WorkerAgent::GATHER_GAS);
				assignedWorkers.push_back(worker);
			}
		}
	}
}

string AssimilatorAgent::getTypeName() {
	return "AssimilatorAgent";
}
