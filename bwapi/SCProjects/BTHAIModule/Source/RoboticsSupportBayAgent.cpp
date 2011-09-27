#include "RoboticsSupportBayAgent.h"
#include "AgentManager.h"

RoboticsSupportBayAgent::RoboticsSupportBayAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("RoboticsSupportBayAgent created (%s)", unit->getType().getName().c_str());

	upgradeOrder.push_back(UpgradeTypes::Scarab_Damage);
	upgradeOrder.push_back(UpgradeTypes::Reaver_Capacity);
	upgradeOrder.push_back(UpgradeTypes::Gravitic_Drive);
}

void RoboticsSupportBayAgent::computeActions() {
	//Wait with upgrades until we have a couple of Reavers.
	if (AgentManager::getInstance()->countNoUnits(UnitTypes::Protoss_Reaver) >= 4) {
		doUpgrades();
	}
}

string RoboticsSupportBayAgent::getTypeName() {
	return "RoboticsSupportBayAgent";
}



