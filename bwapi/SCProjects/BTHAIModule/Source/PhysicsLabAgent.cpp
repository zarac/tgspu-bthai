#include "PhysicsLabAgent.h"
#include "AgentManager.h"

PhysicsLabAgent::PhysicsLabAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("PhysicsLabAgent created (%s)", unit->getType().getName().c_str());

	researchOrder.push_back(TechTypes::Yamato_Gun);
}

void PhysicsLabAgent::computeActions() {
	doResearch();
}

string PhysicsLabAgent::getTypeName() {
	return "PhysicsLabAgent";
}



