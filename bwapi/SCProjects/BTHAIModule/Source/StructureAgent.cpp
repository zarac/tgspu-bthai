#include "StructureAgent.h"
#include "AgentManager.h"

StructureAgent::StructureAgent() {

}

StructureAgent::StructureAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("StructureAgent created (%s)", unit->getType().getName().c_str());
}

string StructureAgent::getTypeName() {
	return "StructureAgent";
}

bool StructureAgent::canResearch(TechType type) {
	if (unit->isBeingConstructed()) {
		return false;
	}

	if (unit->isIdle() && !BuildPlanner::getInstance()->isLocked()) {
		if (Broodwar->canResearch(unit, type)) {
			return true;
		}
	}
	return false;
}

bool StructureAgent::canUpgrade(UpgradeType type) {
	if (unit->isBeingConstructed()) {
		return false;
	}

	if (unit->isIdle() && !BuildPlanner::getInstance()->isLocked()) {
		if (Broodwar->canUpgrade(unit, type)) {
			return true;
		}
	}
	return false;
}

void StructureAgent::doResearch() {
	if ((int)researchOrder.size() > 0) {
		if (canResearch(researchOrder.at(0))) {
			unit->research(researchOrder.at(0));
			researchOrder.erase(researchOrder.begin());
		}
	}
}

void StructureAgent::doUpgrades() {
	if ((int)upgradeOrder.size() > 0) {
		if (canUpgrade(upgradeOrder.at(0))) {
			unit->upgrade(upgradeOrder.at(0));
			upgradeOrder.erase(upgradeOrder.begin());
		}
	}
}

void StructureAgent::printInfo() {
	Broodwar->printf("[%d] (%s) %s", unitID, getTypeName().c_str(), info.c_str());
}

