#include "ArmoryAgent.h"
#include "AgentManager.h"

ArmoryAgent::ArmoryAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("ArmoryAgent created (%s)", unit->getType().getName().c_str());

	upgradeOrder.push_back(UpgradeTypes::Terran_Vehicle_Weapons);
	upgradeOrder.push_back(UpgradeTypes::Terran_Vehicle_Weapons);
	upgradeOrder.push_back(UpgradeTypes::Terran_Vehicle_Weapons);
	//upgradeOrder.push_back(UpgradeTypes::Terran_Vehicle_Plating);
}

void ArmoryAgent::computeActions() {
	doUpgrades();
}

string ArmoryAgent::getTypeName() {
	return "ArmoryAgent";
}



