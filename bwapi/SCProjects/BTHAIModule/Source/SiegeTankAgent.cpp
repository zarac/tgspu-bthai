#include "SiegeTankAgent.h"
#include "PFManager.h"
#include "ExplorationManager.h"
#include "SpottedObject.h"

SiegeTankAgent::SiegeTankAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("SiegeTankAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;

	goal = TilePosition(-1, -1);
}

void SiegeTankAgent::computeActions() {
	int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastFrame < 20) {
		return;
	}
	lastFrame = cFrame; 

	//TODO: Add unitspecific code here

	//The tank cant move if sieged
	if (!unit->isSieged()) {
		PFManager::getInstance()->computeAttackingUnitActions(this, goal, false);
	}
}

string SiegeTankAgent::getTypeName() {
	return "SiegeTankAgent";
}
