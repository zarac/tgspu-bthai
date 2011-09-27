#include "CorsairAgent.h"
#include "PFManager.h"
#include "AgentManager.h"

CorsairAgent::CorsairAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("CorsairAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;

	goal = TilePosition(-1, -1);
}

void CorsairAgent::computeActions() {
	int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastFrame < 20) {
		return;
	}
	lastFrame = cFrame;

	//TODO: Add unitspecific code here

	bool defensive = false;

	PFManager::getInstance()->computeAttackingUnitActions(this, goal, defensive);
}

string CorsairAgent::getTypeName() {
	return "CorsairAgent";
}
