#include "CreepColonyAgent.h"
#include "WorkerAgent.h"
#include "AgentManager.h"

CreepColonyAgent::CreepColonyAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("CreepColonyAgent created (%s)", unit->getType().getName().c_str());
}

void CreepColonyAgent::computeActions() {
	// TODO : use environment
	unit->morph(UnitTypes::Zerg_Sunken_Colony);

}

string CreepColonyAgent::getTypeName() {
	return "CreepColonyAgent";
}
