#include "ArbiterAgent.h"
#include "PFManager.h"
#include "AgentManager.h"

ArbiterAgent::ArbiterAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("ArbiterAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;

	goal = TilePosition(-1, -1);
}

void ArbiterAgent::computeActions() {
	int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastFrame < 20) {
		return;
	}
	lastFrame = cFrame;

	bool defensive = false;
	int eCnt = this->enemyAttackingUnitsWithinRange();
	if (eCnt > 0) {
		int fCnt = friendlyUnitsWithinRange();
		if (fCnt < 5) {
			defensive = true;
		}
	}
	
	//TODO: Add unitspecific code here

	PFManager::getInstance()->computeAttackingUnitActions(this, goal, defensive);
}

string ArbiterAgent::getTypeName() {
	return "ArbiterAgent";
}
