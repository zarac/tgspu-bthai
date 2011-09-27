#include "ScienceFacilityAgent.h"
#include "AgentManager.h"

ScienceFacilityAgent::ScienceFacilityAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("ScienceFacilityAgent created (%s)", unit->getType().getName().c_str());
	hasAddon = 0;

	researchOrder.push_back(TechTypes::EMP_Shockwave);
	researchOrder.push_back(TechTypes::Irradiate);
}

void ScienceFacilityAgent::computeActions() {
	if (hasAddon < 2) {
		if (AgentManager::getInstance()->countNoUnits(toBuild) > 0) {
			hasAddon = 2;
		}
	}
	if (hasAddon == 1) {
		if (AgentManager::getInstance()->countNoUnits(toBuild) == 0) {
			//Broodwar->printf("Addon failed. Trying again");
			unit->buildAddon(toBuild);
		}
	}

	if (hasAddon == 2) {
		doResearch();
	}
}

void ScienceFacilityAgent::assignToBuild(UnitType type) {
	toBuild = type;
	unit->buildAddon(type);
	hasAddon = 1;
}

string ScienceFacilityAgent::getTypeName() {
	return "ScienceFacilityAgent";
}
