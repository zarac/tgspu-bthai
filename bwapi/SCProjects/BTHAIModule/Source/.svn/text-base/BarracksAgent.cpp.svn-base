#include "BarracksAgent.h"
#include "AgentManager.h"
#include "ExplorationManager.h"
#include "Commander.h"

BarracksAgent::BarracksAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("BarracksAgent created (%s)", unit->getType().getName().c_str());
}

void BarracksAgent::computeActions() {
	UnitType marine = UnitTypes::Terran_Marine;
	int noAcademy = AgentManager::getInstance()->countNoUnits(UnitTypes::Terran_Academy);

	if (Broodwar->self()->hasResearched(TechTypes::Personnel_Cloaking)) {
		UnitType ghost = UnitTypes::Terran_Ghost;
		if (Commander::getInstance()->needUnit(ghost)) {
			if (canBuild(ghost)) {
				unit->train(ghost);
				return;
			}
		}
	}

	if (noAcademy > 0) {
		UnitType firebat = UnitTypes::Terran_Firebat;
		if (Commander::getInstance()->needUnit(firebat)) {
			if (canBuild(firebat)) {
				unit->train(firebat);
				return;
			}
		}
	}

	if (noAcademy > 0) {
		UnitType medic = UnitTypes::Terran_Medic;
		if (Commander::getInstance()->needUnit(medic)) {
			if (canBuild(medic)) {
				unit->train(medic);
				return;
			}
		}
	}

	if (Commander::getInstance()->needUnit(marine)) {
		if (canBuild(marine)) {
			unit->train(marine);
			return;
		}
	}
}

string BarracksAgent::getTypeName() {
	return "BarracksAgent";
}
