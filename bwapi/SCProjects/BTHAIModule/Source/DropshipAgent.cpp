#include "DropshipAgent.h"
#include "PFManager.h"
#include "AgentManager.h"

DropshipAgent::DropshipAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("DropshipAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;

	goal = TilePosition(-1, -1);
}

void DropshipAgent::computeActions() {
	int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastFrame < 20) {
		return;
	}
	lastFrame = cFrame;

	bool defensive = true;
	
	//TODO: Add unitspecific code here
	
	PFManager::getInstance()->computeAttackingUnitActions(this, goal, defensive);
}

string DropshipAgent::getTypeName() {
	return "DropshipAgent";
}
