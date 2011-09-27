#include "ValkyrieAgent.h"
#include "PFManager.h"
#include "AgentManager.h"

ValkyrieAgent::ValkyrieAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("ValkyrieAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;

	goal = TilePosition(-1, -1);
}

void ValkyrieAgent::computeActions() {
	int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastFrame < 20) {
		return;
	}
	lastFrame = cFrame;

	bool defensive = false;

	int eCnt = enemyAirUnitsWithinRange(unit->getType().airWeapon().maxRange());
	int fCnt = friendlyUnitsWithinRange();

	if (eCnt > fCnt) {
		defensive = true;
	}
	
	//TODO: Add unitspecific code here
	
	PFManager::getInstance()->computeAttackingUnitActions(this, goal, defensive);
}

string ValkyrieAgent::getTypeName() {
	return "ValkyrieAgent";
}
