#include "ScoutAgent.h"
#include "PFManager.h"
#include "AgentManager.h"

ScoutAgent::ScoutAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("ScoutAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;

	goal = TilePosition(-1, -1);
}

void ScoutAgent::computeActions() {
	int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastFrame < 20) {
		return;
	}
	lastFrame = cFrame;

	bool defensive = false;
	
	//TODO: Add unitspecific code here
	
	PFManager::getInstance()->computeAttackingUnitActions(this, goal, defensive);
}

string ScoutAgent::getTypeName() {
	return "ScoutAgent";
}
