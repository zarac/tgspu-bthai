#include "DragoonAgent.h"
#include "PFManager.h"
#include "AgentManager.h"

DragoonAgent::DragoonAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("DragoonAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;

	goal = TilePosition(-1, -1);
}

void DragoonAgent::computeActions() {
	int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastFrame < 20) {
		return;
	}
	lastFrame = cFrame;

	bool defensive = false;

	//TODO: Add unitspecific code here

	PFManager::getInstance()->computeAttackingUnitActions(this, goal, defensive);
}

string DragoonAgent::getTypeName() {
	return "DragoonAgent";
}
