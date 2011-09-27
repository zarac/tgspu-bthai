#include "WraithAgent.h"
#include "PFManager.h"
#include "AgentManager.h"

WraithAgent::WraithAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("WraithAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;

	goal = TilePosition(-1, -1);
}

void WraithAgent::computeActions() {
	int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastFrame < 20) {
		return;
	}
	lastFrame = cFrame;

	if (currentState != EXPLORE) {
		bool defensive = false;
		int eCnt = this->enemyAttackingUnitsWithinRange(11 * 32);
		if (eCnt > 0) {
			defensive = true;
		}
		if (unit->isCloaked() && !unit->isDetected()) {
			defensive = false;
		}
		PFManager::getInstance()->computeAttackingUnitActions(this, goal, false);
	}
	else {
		//TODO: Add unitspecific code here
		
		if (goal.x() >= 0) {
			bool ok = unit->rightClick(Position(goal));
			if (!ok) {
				Broodwar->printf("[%d] Explorer unable to reach goal", unitID);
			}
		}
	}
}

string WraithAgent::getTypeName() {
	return "WraithAgent";
}
