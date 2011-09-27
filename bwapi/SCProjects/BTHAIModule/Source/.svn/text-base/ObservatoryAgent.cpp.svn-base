#include "ObservatoryAgent.h"
#include "AgentManager.h"

ObservatoryAgent::ObservatoryAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("ObservatoryAgent created (%s)", unit->getType().getName().c_str());

	upgradeOrder.push_back(UpgradeTypes::Sensor_Array);
	upgradeOrder.push_back(UpgradeTypes::Gravitic_Boosters);
}

void ObservatoryAgent::computeActions() {
	doUpgrades();
}

string ObservatoryAgent::getTypeName() {
	return "ObservatoryAgent";
}



