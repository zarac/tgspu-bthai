#include "ComsatAgent.h"
#include "AgentManager.h"

ComsatAgent::ComsatAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("ComsatAgent created (%s)", unit->getType().getName().c_str());
	lastSweepFrame = 0;
}

void ComsatAgent::computeActions() {
	
}

string ComsatAgent::getTypeName() {
	return "ComsatAgent";
}

bool ComsatAgent::sweepArea(TilePosition target) {
	if (target.x() < 0) {
		return false;
	}

	int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastSweepFrame < 30) {
		return false;
	}

	TechType scan = TechTypes::Scanner_Sweep;
	if (unit->getEnergy() >= 50) {
		Broodwar->printf("[%d] Using Scanner Sweep on (%d,%d)", unitID, target.x(), target.y());
		lastSweepFrame = Broodwar->getFrameCount();
		unit->useTech(scan, Position(target));
		return true;
	}

	return false;
}



