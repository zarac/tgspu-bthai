#include "OverlordAgent.h"
#include "PFManager.h"
#include "AgentManager.h"

OverlordAgent::OverlordAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("ZealotAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;

	goal = TilePosition(-1, -1);
	assignToExplore();
}

void OverlordAgent::computeActions() {
	int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastFrame < 20) {
		return;
	}
	lastFrame = cFrame;

	if (currentState != EXPLORE) {
		bool defensive = false;

		//TODO: Add unitspecific code here

		PFManager::getInstance()->computeAttackingUnitActions(this, goal, defensive);
	}
	else {
		if (goal.x() < 0)
		{
			goal = TilePosition(getClosestChokePoint()->getCenter());
			unit->rightClick(Position(goal));
		}
		else if (goal.x() == TilePosition(unit->getPosition()).x() &&
			goal.y() == TilePosition(unit->getPosition()).y())
		{
			Broodwar->printf("getting new choke point");
			int chokepointNo = BuildPlanner::getInstance()->getChokepointNo();
			BWTA::Chokepoint* choke = BaseAgent::getChokePoint(chokepointNo);
			if (choke != NULL) {
				Broodwar->printf("... got one!");
				goal = TilePosition(choke->getCenter());
				unit->rightClick(Position(goal));
			}
			else
				assignToDefend();
			//goal = TilePosition(10, 10);
		}
	}
}

string OverlordAgent::getTypeName() {
	return "OverlordAgent";
}