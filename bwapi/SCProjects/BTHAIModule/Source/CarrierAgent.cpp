#include "CarrierAgent.h"
#include "PFManager.h"
#include "AgentManager.h"

CarrierAgent::CarrierAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("CarrierAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;

	goal = TilePosition(-1, -1);
}

void CarrierAgent::computeActions() {
	int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastFrame < 20) {
		return;
	}
	lastFrame = cFrame;

	int maxLoad = 4;
	if (Broodwar->self()->getUpgradeLevel(UpgradeTypes::Carrier_Capacity) > 0) {
		maxLoad = 8;
	}

	if (unit->getInterceptorCount() < maxLoad) {
		unit->train(UnitTypes::Protoss_Interceptor);
		return;
	}

	bool defensive = false;
	
	//TODO: Add unitspecific code here

	PFManager::getInstance()->computeAttackingUnitActions(this, goal, defensive);
}

string CarrierAgent::getTypeName() {
	return "CarrierAgent";
}
