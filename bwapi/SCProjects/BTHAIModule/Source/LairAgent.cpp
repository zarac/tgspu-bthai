#include "LairAgent.h"
#include "AgentManager.h"
#include "WorkerAgent.h"
#include "PFManager.h"
#include "ZergBuildPlanner.h"

LairAgent::LairAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("LairAgent created (%s)", unit->getType().getName().c_str());

	if (analyzed) {
		home = BWTA::getRegion(unit->getTilePosition());
	}

	researchOrder.push_back(TechTypes::Burrowing);
}

void LairAgent::computeActions() {
	doResearch();

	if ((int)researchOrder.size() == 0) {
		doUpgrades();
	}

	int level = ((ZergCommander*)Commander::getInstance())->getLevel();
	if (level >= 5)
	{
		if (canUpgrade(UpgradeTypes::Ventral_Sacs))
			unit->upgrade(UpgradeTypes::Ventral_Sacs);		
		else if (canUpgrade(UpgradeTypes::Antennae))
			unit->upgrade(UpgradeTypes::Antennae);
		else if (canUpgrade(UpgradeTypes::Pneumatized_Carapace))
			unit->upgrade(UpgradeTypes::Pneumatized_Carapace);
		return;
	}


	//// upgrade to lair if level 5
	//int level = ((ZergCommander*)Commander::getInstance())->getLevel();
	//if (level >= 5 && unit->getType() != UnitTypes::Zerg_Lair)
	//{
	//	unit->upgrade(UpgradeTypes::Zer);
	//}

	// build drones
	ZergCommander* zc = ((ZergCommander*)Commander::getInstance());
	UnitType worker = Broodwar->self()->getRace().getWorker();
	while (zc->getWorkersNeeded() > 0 && canBuild(worker))
	{
		unit->train(worker);
		zc->setWorkersNeeded(zc->getWorkersNeeded()-1);
	}

	// build hydralisks
	UnitType hydralisk = UnitTypes::Zerg_Hydralisk;
	if (Commander::getInstance()->needUnit(hydralisk)) {
		if (canBuild(hydralisk)) {
			unit->train(hydralisk);
			return;
		}
	}

	// build zerglings
	UnitType zergling = UnitTypes::Zerg_Zergling;
	if (Commander::getInstance()->needUnit(zergling)) {
		if (canBuild(zergling)) {
			unit->train(zergling);
			return;
		}
	}
}

string LairAgent::getTypeName() {
	return "LairAgent";
}