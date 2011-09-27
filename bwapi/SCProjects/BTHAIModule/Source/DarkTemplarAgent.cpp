#include "DarkTemplarAgent.h"
#include "PFManager.h"
#include "AgentManager.h"

DarkTemplarAgent::DarkTemplarAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("DarkTemplarAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;

	goal = TilePosition(-1, -1);
}

void DarkTemplarAgent::computeActions() {
	int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastFrame < 20) {
		return;
	}
	lastFrame = cFrame;

	bool defensive = false;
	
	//TODO: Add unitspecific code here

	PFManager::getInstance()->computeAttackingUnitActions(this, goal, defensive);
}

string DarkTemplarAgent::getTypeName() {
	return "DarkTemplarAgent";
}

