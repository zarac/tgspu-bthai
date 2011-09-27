#include "CitadelOfAdunAgent.h"
#include "AgentManager.h"

CitadelOfAdunAgent::CitadelOfAdunAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("CitadelOfAdunAgent created (%s)", unit->getType().getName().c_str());

	upgradeOrder.push_back(UpgradeTypes::Leg_Enhancements);
}

void CitadelOfAdunAgent::computeActions() {
	doUpgrades();
}

string CitadelOfAdunAgent::getTypeName() {
	return "CitadelOfAdunAgent";
}



