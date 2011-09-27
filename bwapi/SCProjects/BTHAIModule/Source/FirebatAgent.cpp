#include "FirebatAgent.h"
#include "PFManager.h"
#include "AgentManager.h"
#include "BunkerAgent.h"

FirebatAgent::FirebatAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("FirebatAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;

	goal = TilePosition(-1, -1);
}

void FirebatAgent::computeActions() {
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

	if (currentState == ATTACK || currentState == DEFEND) {
		bool defensive = false;
		
		//TODO: Add unitspecific code here
		
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

bool FirebatAgent::isBunkered() {
	if (currentState == BUNKERED) {
		return true;
	}
	return false;
}

void FirebatAgent::assignToBunkerDefend(Unit* bunker) {
	currentState = BUNKERED;
	unit->rightClick(bunker);
	myBunker = bunker;
}

string FirebatAgent::getTypeName() {
	return "FirebatAgent";
}
