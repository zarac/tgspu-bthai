#include "ZerglingAgent.h"
#include "PFManager.h"
#include "AgentManager.h"

ZerglingAgent::ZerglingAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("ZealotAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;

	goal = TilePosition(-1, -1);
}

void ZerglingAgent::computeActions() {
	int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastFrame < 20) {
		return;
	}
	lastFrame = cFrame;

	if (currentState != EXPLORE) {
		bool defensive = false;

		//TODO: Add unitspecific code here

		PFManager::getInstance()->computeAttackingUnitActions(this, goal, defensive);
	}
	else {
		if (goal.x() >= 0) {
			unit->rightClick(Position(goal));
		}
	}
}

string ZerglingAgent::getTypeName() {
	return "ZerglingAgent";
}