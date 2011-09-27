#include "RoboticsFacilityAgent.h"
#include "AgentManager.h"
#include "Commander.h"

RoboticsFacilityAgent::RoboticsFacilityAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("RoboticsFacilityAgent created (%s)", unit->getType().getName().c_str());
}

void RoboticsFacilityAgent::computeActions() {
	if (AgentManager::getInstance()->countNoUnits(UnitTypes::Protoss_Robotics_Support_Bay) > 0) {
		if (Commander::getInstance()->needUnit(UnitTypes::Protoss_Reaver)) {
			if (canBuild(UnitTypes::Protoss_Reaver)) {
				unit->train(UnitTypes::Protoss_Reaver);
				return;
			}
		}
	}

	if (AgentManager::getInstance()->countNoUnits(UnitTypes::Protoss_Observatory) > 0) {
		if (Commander::getInstance()->needUnit(UnitTypes::Protoss_Observer)) {
			if (canBuild(UnitTypes::Protoss_Observer)) {
				unit->train(UnitTypes::Protoss_Observer);
				return;
			}
		}
	}
}

string RoboticsFacilityAgent::getTypeName() {
	return "RoboticsFacilityAgent";
}
