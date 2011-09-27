#include "AcademyAgent.h"
#include "AgentManager.h"

AcademyAgent::AcademyAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("AcademyAgent created (%s)", unit->getType().getName().c_str());

	upgradeOrder.push_back(UpgradeTypes::U_238_Shells);
	researchOrder.push_back(TechTypes::Stim_Packs);
}

void AcademyAgent::computeActions() {
	doResearch();

	if ((int)researchOrder.size() == 0) {
		doUpgrades();
	}
}

string AcademyAgent::getTypeName() {
	return "AcademyAgent";
}



