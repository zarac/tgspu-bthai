#include "SunkenColonyAgent.h"
#include "WorkerAgent.h"
#include "AgentManager.h"

SunkenColonyAgent::SunkenColonyAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("SunkenColonyAgent created (%s)", unit->getType().getName().c_str());
}

void SunkenColonyAgent::computeActions() {
}

string SunkenColonyAgent::getTypeName() {
	return "SunkenColonyAgent";
}