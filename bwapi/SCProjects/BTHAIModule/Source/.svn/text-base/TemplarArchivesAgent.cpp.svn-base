#include "TemplarArchivesAgent.h"
#include "AgentManager.h"

TemplarArchivesAgent::TemplarArchivesAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("TemplarArchivesAgent created (%s)", unit->getType().getName().c_str());

	//High Templar abilities
	researchOrder.push_back(TechTypes::Psionic_Storm);
	researchOrder.push_back(TechTypes::Hallucination);
	//Dark Archon abilities
	//researchOrder.push_back(TechTypes::Maelstrom);
	//researchOrder.push_back(TechTypes::Mind_Control);
	
	//+50 High Templar energy
	//upgradeOrder.push_back(UpgradeTypes::Khaydarin_Amulet);
	//+50 Dark Archons energy
	//upgradeOrder.push_back(UpgradeTypes::Argus_Talisman);
}

void TemplarArchivesAgent::computeActions() {
	doResearch();

	if ((int)researchOrder.size() == 0) {
		doUpgrades();
	}
}

string TemplarArchivesAgent::getTypeName() {
	return "TemplarArchivesAgent";
}
