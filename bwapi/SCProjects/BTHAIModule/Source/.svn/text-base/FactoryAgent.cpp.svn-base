#include "FactoryAgent.h"
#include "AgentManager.h"
#include "Commander.h"

FactoryAgent::FactoryAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("FactoryAgent created (%s)", unit->getType().getName().c_str());
	hasMachineShop = 0;
}

void FactoryAgent::computeActions() {
	if (hasMachineShop < 2) {
		if (AgentManager::getInstance()->countNoUnits(UnitTypes::Terran_Machine_Shop) > 0) {
			hasMachineShop = 2;
		}
	}
	if (hasMachineShop == 1) {
		if (AgentManager::getInstance()->countNoUnits(UnitTypes::Terran_Machine_Shop) == 0) {
			//Broodwar->printf("MachineShop addon failed. Trying again");
			unit->buildAddon(UnitTypes::Terran_Machine_Shop);
		}
	}

	if (hasMachineShop == 2) {
		if (Commander::getInstance()->needUnit(UnitTypes::Terran_Siege_Tank_Tank_Mode)) {
			if (canBuild(UnitTypes::Terran_Siege_Tank_Tank_Mode)) {
				unit->train(UnitTypes::Terran_Siege_Tank_Tank_Mode);
				return;
			}
		}

		if (Commander::getInstance()->needUnit(UnitTypes::Terran_Vulture)) {
			if (canBuild(UnitTypes::Terran_Vulture)) {
				unit->train(UnitTypes::Terran_Vulture);
				return;
			}
		}
	}
}

void FactoryAgent::assignToBuild(UnitType type) {
	unit->buildAddon(UnitTypes::Terran_Machine_Shop);
	hasMachineShop = 1;
}

string FactoryAgent::getTypeName() {
	return "FactoryAgent";
}

void FactoryAgent::printInfo() {
	string locked = "No";
	if (BuildPlanner::getInstance()->isLocked()) {
		locked = "Yes";
	}
	Broodwar->printf("[%d] %s Locked: %s", unitID, getTypeName().c_str(), locked.c_str());
}
