#include "MachineShopAgent.h"
#include "AgentManager.h"

MachineShopAgent::MachineShopAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("MachineShopAgent created (%s)", unit->getType().getName().c_str());

	researchOrder.push_back(TechTypes::Tank_Siege_Mode);
	researchOrder.push_back(TechTypes::Spider_Mines);
	
	upgradeOrder.push_back(UpgradeTypes::Ion_Thrusters);
}

void MachineShopAgent::computeActions() {
	doResearch();

	if ((int)researchOrder.size() == 0) {
		//Ion Thrusters is the last thing to do
		doUpgrades();
	}
}

string MachineShopAgent::getTypeName() {
	return "MachineShopAgent";
}



