#include "GatewayAgent.h"
#include "AgentManager.h"
#include "Commander.h"

GatewayAgent::GatewayAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("GatewayAgent created (%s)", unit->getType().getName().c_str());
}

void GatewayAgent::computeActions() {
	UnitType zealot = UnitTypes::Protoss_Zealot;
	
	if (Commander::getInstance()->needUnit(zealot)) {
		if (canBuild(zealot)) {
			unit->train(zealot);
			return;
		}
	}

	if (AgentManager::getInstance()->countNoUnits(UnitTypes::Protoss_Cybernetics_Core) > 0) {
		UnitType dragoon = UnitTypes::Protoss_Dragoon;
		if (Commander::getInstance()->needUnit(dragoon)) {
			if (canBuild(dragoon)) {
				unit->train(dragoon);
				return;
			}
		}
	}

	if (AgentManager::getInstance()->countNoUnits(UnitTypes::Protoss_Templar_Archives) > 0) {
		UnitType highTemplar = UnitTypes::Protoss_High_Templar;
		if (Commander::getInstance()->needUnit(highTemplar)) {
			if (canBuild(highTemplar)) {
				unit->train(highTemplar);
				return;
			}
		}

		UnitType darkTemplar = UnitTypes::Protoss_Dark_Templar;
		if (Commander::getInstance()->needUnit(darkTemplar)) {
			if (canBuild(darkTemplar)) {
				unit->train(darkTemplar);
				return;
			}
		}
	}
}

string GatewayAgent::getTypeName() {
	return "GatewayAgent";
}
