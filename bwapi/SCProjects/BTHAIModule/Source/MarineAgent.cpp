#include "MarineAgent.h"
#include "PFManager.h"
#include "AgentManager.h"
#include "BunkerAgent.h"

MarineAgent::MarineAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("MarineAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;

	goal = TilePosition(-1, -1);
}

void MarineAgent::computeActions() {
	int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastFrame < 20) {
		return;
	}
	lastFrame = cFrame;

	if (currentState == BUNKERED) {
		if (!myBunker->exists()) {
			currentState = DEFEND;
		}
	}
	
	//TODO: Add unitspecific code here

	if (currentState == ATTACK || currentState == DEFEND) {
		//NOTE: This is an example of how special abilities are used.
		if (Broodwar->self()->hasResearched(TechTypes::Stim_Packs)) {
			if (!unit->isStimmed() && unit->getHitPoints() >= 20) {
				int eCnt = enemyAttackingUnitsWithinRange();
				if (eCnt > 0) {
					unit->useTech(TechTypes::Stim_Packs);
					//Broodwar->printf("[%d] Using stim packs", unitID);
					return;
				}
			}
		}

		bool defensive = false;
		int eCnt = enemyAttackingUnitsWithinRange();
		int fCnt = friendlyUnitsWithinRange();
		if (eCnt > fCnt) {
			defensive = true;
		}

		PFManager::getInstance()->computeAttackingUnitActions(this, goal, defensive);
	}

	if (currentState == EXPLORE) {
		if (goal.x() >= 0) {
			bool ok = unit->rightClick(Position(goal));
			if (!ok) {
				Broodwar->printf("[%d] Explorer unable to reach goal", unitID);
			}
		}
	}
}

bool MarineAgent::isBunkered() {
	if (currentState == BUNKERED) {
		return true;
	}
	return false;
}

void MarineAgent::assignToBunkerDefend(Unit* bunker) {
	currentState = BUNKERED;
	unit->rightClick(bunker);
	myBunker = bunker;
}

string MarineAgent::getTypeName() {
	return "MarineAgent";
}
