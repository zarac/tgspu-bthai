#include "CovertOpsAgent.h"
#include "AgentManager.h"

CovertOpsAgent::CovertOpsAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("CovertOpsAgent created (%s)", unit->getType().getName().c_str());

	researchOrder.push_back(TechTypes::Personnel_Cloaking);
	researchOrder.push_back(TechTypes::Lockdown);
	
	upgradeOrder.push_back(UpgradeTypes::Ocular_Implants);
	upgradeOrder.push_back(UpgradeTypes::Moebius_Reactor);
}

void CovertOpsAgent::computeActions() {
	doResearch();

	if ((int)researchOrder.size() == 0) {
		doUpgrades();
	}
}

string CovertOpsAgent::getTypeName() {
	return "CovertOpsAgent";
}



