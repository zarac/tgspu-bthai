#include "ReaverAgent.h"
#include "PFManager.h"
#include "AgentManager.h"

ReaverAgent::ReaverAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("ReaverAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;

	goal = TilePosition(-1, -1);
	goalSet = false;
}

void ReaverAgent::computeActions() {
	int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastFrame < 20) {
		return;
	}
	lastFrame = cFrame;

	bool defensive = false;

	//TODO: Add unitspecific code here

	PFManager::getInstance()->computeAttackingUnitActions(this, goal, defensive);
}

void ReaverAgent::setGoal(TilePosition goal) {
	//Do nothing
}

void ReaverAgent::clearGoal() {
	//Do nothing
}

string ReaverAgent::getTypeName() {
	return "ReaverAgent";
}
