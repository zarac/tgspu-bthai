#include "ArbiterTribunalAgent.h"
#include "AgentManager.h"

ArbiterTribunalAgent::ArbiterTribunalAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("ArbiterTribunalAgent created (%s)", unit->getType().getName().c_str());

	researchOrder.push_back(TechTypes::Stasis_Field);
}

void ArbiterTribunalAgent::computeActions() {
	doResearch();
}

string ArbiterTribunalAgent::getTypeName() {
	return "ArbiterTribunalAgent";
}



