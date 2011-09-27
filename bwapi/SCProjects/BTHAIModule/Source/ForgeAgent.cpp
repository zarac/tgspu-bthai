#include "ForgeAgent.h"
#include "AgentManager.h"

ForgeAgent::ForgeAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("ForgeAgent created (%s)", unit->getType().getName().c_str());

	upgradeOrder.push_back(UpgradeTypes::Protoss_Ground_Weapons);
	upgradeOrder.push_back(UpgradeTypes::Protoss_Ground_Weapons);
	upgradeOrder.push_back(UpgradeTypes::Protoss_Ground_Weapons);
	level = 1;

	//upgradeOrder.push_back(UpgradeTypes::Protoss_Ground_Armor);
	//upgradeOrder.push_back(UpgradeTypes::Protoss_Plasma_Shields);
}

void ForgeAgent::computeActions() {
	doUpgrades();
}

string ForgeAgent::getTypeName() {
	return "ForgeAgent";
}



