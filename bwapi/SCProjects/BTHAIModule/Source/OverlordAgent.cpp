#include "OverlordAgent.h"
#include "PFManager.h"
#include "AgentManager.h"
#include "Logger.h"

OverlordAgent::OverlordAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("ZealotAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;

	goal = TilePosition(-1, -1);
	assignToExplore();
}

void OverlordAgent::computeActions() {
	/*int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastFrame < 20) {
		return;
	}
	lastFrame = cFrame;*/

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

			//BWTA::getChokepoints
			//* get next choke point
			//std::set<BWTA::Chokepoint*> chokepoints = home->getChokepoints();
			std::set<BWTA::Chokepoint*> chokepoints = BWTA::getChokepoints();
			double distance = 5;
			double min_length=10000;
			BWTA::Chokepoint* choke=NULL;
			//iterate through all chokepoints and look for the one with the smallest gap (least width)
			Logger::Log("Looking for choke point");
			for (std::set<BWTA::Chokepoint*>::iterator c = chokepoints.begin(); c != chokepoints.end(); c++) {
				//Logger::Log("checking poinnnttt");
				double length = (*c)->getWidth();
				if (distance < length && (length < min_length || choke == NULL)) {
					Logger::Log("Found closer choke point");
					min_length = length;
					choke = *c;
				}
			}

			Logger::Log("getting new choke point");
			//int chokepointNo = getClosestChokePoint(5);
			//int chokepointNo = BuildPlanner::getInstance()->getChokepointNo();
			//BWTA::Chokepoint* choke = BaseAgent::getChokePoint(chokepointNo);
			//BWTA::Chokepoint* choke = getClosestChokePoint(20);
			if (choke != NULL) {
				Logger::Log("... got one!");
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