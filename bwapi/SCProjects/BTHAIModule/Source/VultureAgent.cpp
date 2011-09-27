#include "VultureAgent.h"
#include "PFManager.h"
#include "AgentManager.h"

VultureAgent::VultureAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("VultureAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;
	mineDropFrame = 0;

	goal = TilePosition(-1, -1);
}

void VultureAgent::computeActions() {
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

string VultureAgent::getTypeName() {
	return "VultureAgent";
}

