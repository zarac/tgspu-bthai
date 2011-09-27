#include "ShieldBatteryAgent.h"
#include "AgentManager.h"

ShieldBatteryAgent::ShieldBatteryAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("ShieldBatteryAgent created (%s)", unit->getType().getName().c_str());
}

void ShieldBatteryAgent::computeActions() {
	//TODO: Add unitspecific code here
}

string ShieldBatteryAgent::getTypeName() {
	return "ShieldBatteryAgent";
}



