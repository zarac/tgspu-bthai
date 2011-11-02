#include "SpawningPoolAgent.h"
#include "WorkerAgent.h"
#include "AgentManager.h"

SpawningPoolAgent::SpawningPoolAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("SpawningPoolAgent created (%s)", unit->getType().getName().c_str());
	
	//researchOrder.push_back(UpgradeTypes::Metabolic_Boost);
}

void SpawningPoolAgent::computeActions() {
	//doResearch();

	//if ((int)researchOrder.size() == 0) {
	//	doUpgrades();
	//}
	if (canUpgrade(UpgradeTypes::Metabolic_Boost))
		unit->upgrade(UpgradeTypes::Metabolic_Boost);
	if (canUpgrade(UpgradeTypes::Adrenal_Glands))
		unit->upgrade(UpgradeTypes::Adrenal_Glands);
}

string SpawningPoolAgent::getTypeName() {
	return "SpawningPoolAgent";
}
