#include "CyberneticsCoreAgent.h"
#include "AgentManager.h"

CyberneticsCoreAgent::CyberneticsCoreAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("CyberneticsCoreAgent created (%s)", unit->getType().getName().c_str());

	upgradeOrder.push_back(UpgradeTypes::Singularity_Charge);
	//upgradeOrder.push_back(UpgradeTypes::Protoss_Air_Weapons);
	//upgradeOrder.push_back(UpgradeTypes::Protoss_Air_Armor);
}

void CyberneticsCoreAgent::computeActions() {
	doUpgrades();
}

string CyberneticsCoreAgent::getTypeName() {
	return "CyberneticsCoreAgent";
}



