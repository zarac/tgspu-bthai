#include "EngineeringBayAgent.h"
#include "AgentManager.h"

EngineeringBayAgent::EngineeringBayAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("EngineeringBayAgent created (%s)", unit->getType().getName().c_str());

	upgradeOrder.push_back(UpgradeTypes::Terran_Infantry_Weapons);
	upgradeOrder.push_back(UpgradeTypes::Terran_Infantry_Weapons);
	upgradeOrder.push_back(UpgradeTypes::Terran_Infantry_Weapons);
	//upgradeOrder.push_back(UpgradeTypes::Terran_Infantry_Armor);
}

void EngineeringBayAgent::computeActions() {
	doUpgrades();
}

string EngineeringBayAgent::getTypeName() {
	return "EngineeringBayAgent";
}



