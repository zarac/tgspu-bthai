#include "ControlTowerAgent.h"
#include "AgentManager.h"

ControlTowerAgent::ControlTowerAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("ControlTowerAgent created (%s)", unit->getType().getName().c_str());

	researchOrder.push_back(TechTypes::Cloaking_Field);
}

void ControlTowerAgent::computeActions() {
	doResearch();
}

string ControlTowerAgent::getTypeName() {
	return "ControlTowerAgent";
}



