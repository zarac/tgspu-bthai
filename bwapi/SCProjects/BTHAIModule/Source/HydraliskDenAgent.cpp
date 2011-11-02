#include "HydraliskDenAgent.h"
#include "WorkerAgent.h"
#include "AgentManager.h"
#include "ZergCommander.h"

HydraliskDenAgent::HydraliskDenAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("HydraliskDenAgent created (%s)", unit->getType().getName().c_str());
}

void HydraliskDenAgent::computeActions() {
	int level = ((ZergCommander*)Commander::getInstance())->getLevel();
	if (level >= 5)
	{
		if (canUpgrade(UpgradeTypes::Muscular_Augments))
			unit->upgrade(UpgradeTypes::Muscular_Augments);
		
		if (canUpgrade(UpgradeTypes::Grooved_Spines))
			unit->upgrade(UpgradeTypes::Grooved_Spines);
	}
}

string HydraliskDenAgent::getTypeName() {
	return "HydraliskDenAgent";
}
