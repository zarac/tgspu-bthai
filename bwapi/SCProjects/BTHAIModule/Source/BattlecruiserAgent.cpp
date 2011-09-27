#include "BattlecruiserAgent.h"
#include "PFManager.h"
#include "AgentManager.h"

BattlecruiserAgent::BattlecruiserAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("BattlecruiserAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;

	goal = TilePosition(-1, -1);
}

void BattlecruiserAgent::computeActions() {
	int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastFrame < 20) {
		return;
	}
	lastFrame = cFrame;

	//TODO: Add unitspecific code here

	PFManager::getInstance()->computeAttackingUnitActions(this, goal, false);
}

string BattlecruiserAgent::getTypeName() {
	return "BattlecruiserAgent";
}
