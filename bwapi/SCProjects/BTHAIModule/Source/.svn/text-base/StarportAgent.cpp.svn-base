#include "StarportAgent.h"
#include "AgentManager.h"
#include "Commander.h"

StarportAgent::StarportAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("StarportAgent created (%s)", unit->getType().getName().c_str());
	hasAddon = 0;
}

void StarportAgent::computeActions() {
	if (hasAddon < 2) {
		if (AgentManager::getInstance()->countNoUnits(toBuild) > 0) {
			hasAddon = 2;
		}
	}
	if (hasAddon == 1) {
		if (AgentManager::getInstance()->countNoUnits(toBuild) == 0) {
			//Broodwar->printf("Addon addon failed. Trying again");
			unit->buildAddon(toBuild);
		}
	}

	if (hasAddon == 2) {
		if (Commander::getInstance()->needUnit(UnitTypes::Terran_Science_Vessel)) {
			if (canBuild(UnitTypes::Terran_Science_Vessel)) {
				unit->train(UnitTypes::Terran_Science_Vessel);
				return;
			}
		}

		if (Commander::getInstance()->needUnit(UnitTypes::Terran_Wraith)) {
			if (canBuild(UnitTypes::Terran_Wraith)) {
				unit->train(UnitTypes::Terran_Wraith);
				return;
			}
		}
		
		if (Commander::getInstance()->needUnit(UnitTypes::Terran_Battlecruiser)) {
			if (canBuild(UnitTypes::Terran_Battlecruiser)) {
				unit->train(UnitTypes::Terran_Battlecruiser);
				return;
			}
		}
	}
}

void StarportAgent::assignToBuild(UnitType type) {
	toBuild = type;
	unit->buildAddon(type);
	hasAddon = 1;
}

string StarportAgent::getTypeName() {
	return "StarportAgent";
}
