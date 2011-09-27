#include "StargateAgent.h"
#include "AgentManager.h"
#include "Commander.h"

StargateAgent::StargateAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("StargateAgent created (%s)", unit->getType().getName().c_str());
}

void StargateAgent::computeActions() {
	UnitType arbiter = UnitTypes::Protoss_Arbiter;
	UnitType corsair = UnitTypes::Protoss_Corsair;
	UnitType scout = UnitTypes::Protoss_Scout;

	if (AgentManager::getInstance()->countNoUnits(UnitTypes::Protoss_Arbiter_Tribunal) > 0) {
		if (Commander::getInstance()->needUnit(arbiter)) {
			if (canBuild(arbiter)) {
				unit->train(arbiter);
				return;
			}
		}
	}

	if (Commander::getInstance()->needUnit(scout)) {
		if (canBuild(scout)) {
			unit->train(scout);
			return;
		}
	}

	if (AgentManager::getInstance()->countNoUnits(UnitTypes::Protoss_Fleet_Beacon) > 0) {
		if (Commander::getInstance()->needUnit(corsair)) {
			if (canBuild(corsair)) {
				unit->train(corsair);
				return;
			}
		}
	}
}

string StargateAgent::getTypeName() {
	return "StargateAgent";
}
