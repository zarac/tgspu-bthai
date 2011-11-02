#include "EvolutionChamberAgent.h"
#include "WorkerAgent.h"
#include "AgentManager.h"
#include "ZergCommander.h"

EvolutionChamberAgent::EvolutionChamberAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("EvolutionChamberAgent created (%s)", unit->getType().getName().c_str());
}

void EvolutionChamberAgent::computeActions() {
	if (canUpgrade(UpgradeTypes::Zerg_Melee_Attacks))
	{
		unit->upgrade(UpgradeTypes::Zerg_Melee_Attacks);
		//return;
	}
	
	int level = ((ZergCommander*)Commander::getInstance())->getLevel();
	if (level >= 5)
	{
		if (canUpgrade(UpgradeTypes::Zerg_Carapace))
			unit->upgrade(UpgradeTypes::Zerg_Carapace);

		if (canUpgrade(UpgradeTypes::Zerg_Missile_Attacks))
			unit->upgrade(UpgradeTypes::Zerg_Missile_Attacks);
	}
}

string EvolutionChamberAgent::getTypeName() {
	return "EvolutionChamberAgent";
}
