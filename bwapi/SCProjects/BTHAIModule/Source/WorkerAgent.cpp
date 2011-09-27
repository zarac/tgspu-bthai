#include "UnitAgent.h"
#include "WorkerAgent.h"
#include "AgentManager.h"
#include "ExplorationManager.h"
#include "PFManager.h"
#include "CoverMap.h"
#include <sstream>

WorkerAgent::WorkerAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("WorkerAgent created (%s)", unit->getType().getName().c_str());
	setState(GATHER_MINERALS);
	startBuildFrame = 0;
}

void WorkerAgent::computeActions() {
	//Check if workers are too far away from a base when attacking
	if (unit->isAttacking() && unit->getTarget() != NULL) {
		double dist = unit->getDistance(unit->getTarget());
		if (dist >= 20 * 32) {
			//Stop attacking. Return home
			unit->stop();
			BaseAgent* base = AgentManager::getInstance()->getClosestBase(unit->getTilePosition());
			if (base != NULL) {
				unit->rightClick(base->getUnit());
				return;
			}
		}
	}


	if (currentState == MOVE_TO_SPOT || currentState == CONSTRUCT) {
		int w = toBuild.tileWidth() * 32;
		int h = toBuild.tileHeight() * 32;
		Broodwar->drawBox(CoordinateType::Map,buildSpot.x()*32,buildSpot.y()*32,buildSpot.x()*32+w,buildSpot.y()*32+h,Colors::Blue,false);
	}
	
	if (currentState == REPAIRING) {
		if (!unit->isRepairing()) {
			setState(GATHER_MINERALS);
			BaseAgent* base = AgentManager::getInstance()->getClosestBase(unit->getTilePosition());
			if (base != NULL) {
				unit->rightClick(base->getUnit());
				return;
			}
		}
		else {
			return;
		}
	}

	if (currentState == GATHER_MINERALS) {
		if (unit->isIdle()) {
			Unit* closestMineral=NULL;
			for(set<Unit*>::iterator m = Broodwar->getMinerals().begin(); m != Broodwar->getMinerals().end(); m++) {
				if (closestMineral == NULL || unit->getDistance(*m) < unit->getDistance(closestMineral)) {
					closestMineral=*m;
				}
			}
			if (closestMineral != NULL) {
				unit->rightClick(closestMineral);
			}
		}
	}

	if (currentState == FIND_BUILDSPOT) {
		TilePosition startPoint = findStartPoint();
		buildSpot = CoverMap::getInstance()->findBuildSpot(toBuild, startPoint);
		if (buildSpot.x() >= 0) {
			//Broodwar->printf("Build spot for %s found at (%d,%d)", toBuild.getName().c_str(), buildSpot.x(), buildSpot.y());
			setState(MOVE_TO_SPOT);
			startBuildFrame = Broodwar->getFrameCount();
		}
		else {
			Broodwar->printf("[%d] unable to find a build spot for %s", unitID, toBuild.getName().c_str());
			if (toBuild.isResourceDepot()) {
				BuildPlanner::getInstance()->unlock(Broodwar->self()->getRace().getCenter());
			}
			setState(GATHER_MINERALS);
		}
	}

	if (currentState == MOVE_TO_SPOT) {
		//If too much time has elapsed, reset it.
		if (Broodwar->getFrameCount() - startBuildFrame > BuildPlanner::getLockedTimer(toBuild)) {
			reset();
		}

		if (!buildSpotExplored()) {
			//Broodwar->printf("[%d] moving to spot (%d,%d) dist=%d", unitID, buildSpot.x(), buildSpot.y());
			unit->rightClick(Position(buildSpot));
		}

		if (buildSpotExplored() && !unit->isConstructing()) {
			//Broodwar->printf("[%d] start to build at spot (%d,%d)", unitID, buildSpot.x(), buildSpot.y());
			if (areaFree()) {
				unit->build(buildSpot, toBuild);
			}
		}

		if (unit->isConstructing()) {
			//Broodwar->printf("[%d] is building at (%d,%d)", unitID, buildSpot.x(), buildSpot.y());
			setState(CONSTRUCT);
		}
	}

	if (currentState == CONSTRUCT) {
		if (!unit->isConstructing()) {
			//Build finished.
			BaseAgent* agent = AgentManager::getInstance()->getClosestBase(unit->getTilePosition());
			if (agent != NULL) {
				unit->rightClick(agent->getUnit()->getPosition());
			}
			setState(GATHER_MINERALS);
		}
	}
}

bool WorkerAgent::areaFree() {
	if (toBuild.isRefinery()) {
		return true;
	}

	for (int x = buildSpot.x(); x < buildSpot.x() + toBuild.tileWidth(); x++) {
		for (int y = buildSpot.y(); y < buildSpot.y() + toBuild.tileHeight(); y++) {
			for(set<Unit*>::const_iterator i=Broodwar->unitsOnTile(x, y).begin();i!=Broodwar->unitsOnTile(x, y).end();i++) {
				if ((*i)->getID() != unit->getID()) {
					return false;
				}
			}
		}
	}
	return true;
}

TilePosition WorkerAgent::findStartPoint() {
	TilePosition start = Broodwar->self()->getStartLocation();
	BaseAgent* base = AgentManager::getInstance()->getClosestBase(unit->getTilePosition());
	if (base != NULL) {
		start = base->getUnit()->getTilePosition();
	}

	if (BuildPlanner::isProtoss()) {
		if (toBuild.getID() == UnitTypes::Protoss_Pylon.getID()) {

			//If we find unpowered buildings, build a Pylon there
			AgentManager* agentManager = AgentManager::getInstance();
			for (int i = 0; i < (int)agentManager->size(); i++) {
				BaseAgent* agent = agentManager->at(i);
				if (agent->isAlive()) {
					Unit* cUnit = agent->getUnit();
					if (cUnit->isUnpowered()) {
						Broodwar->printf("Build pylon at unpowered building %s", cUnit->getType().getName().c_str());
						start = cUnit->getTilePosition();
						AgentManager::release(agent);
						return start;
					}
				}
				AgentManager::release(agent);
			}
		
			//Build a Pylon at each chokepoint, to power Photon Cannons.
			if (((ProtossBuildPlanner*)BuildPlanner::getInstance())->getPylonAtChokePointNo() < getNoChokePoints()) {
				BWTA::Chokepoint* choke = getChokePoint(((ProtossBuildPlanner*)BuildPlanner::getInstance())->getPylonAtChokePointNo());
				if (choke != NULL) {
					start = TilePosition(choke->getCenter());
					((ProtossBuildPlanner*)BuildPlanner::getInstance())->increasePylonAtChokePointNo();
				}
			}
		}

		if (toBuild.getID() == UnitTypes::Protoss_Shield_Battery.getID()) {
			//Build a Shield Battery at each chokepoint, to regenerate shields.
			if (((ProtossBuildPlanner*)BuildPlanner::getInstance())->getShieldBatteryAtChokePointNo() < getNoChokePoints()) {
				BWTA::Chokepoint* choke = getChokePoint(((ProtossBuildPlanner*)BuildPlanner::getInstance())->getShieldBatteryAtChokePointNo());
				if (choke != NULL) {
					start = TilePosition(choke->getCenter());
					((ProtossBuildPlanner*)BuildPlanner::getInstance())->increaseShieldBatteryAtChokePointNo();
				}
			}
		}
	}

	if (BaseAgent::isOfType(toBuild, UnitTypes::Terran_Supply_Depot)) {
		BaseAgent* depot = AgentManager::getInstance()->getAgent(UnitTypes::Terran_Supply_Depot);
		if (depot != NULL) {
			start = depot->getUnit()->getTilePosition();
		}
	}

	if (BaseAgent::isOfType(toBuild, UnitTypes::Terran_Bunker) || BaseAgent::isOfType(toBuild, UnitTypes::Protoss_Photon_Cannon) || BaseAgent::isOfType(toBuild, UnitTypes::Terran_Missile_Turret)) {
		//Find a good spot
		int chokepointNo = BuildPlanner::getInstance()->getChokepointNo();
		BWTA::Chokepoint* choke = BaseAgent::getChokePoint(chokepointNo);
		if (choke != NULL) {
			start = TilePosition(choke->getCenter());
			Broodwar->printf("[%d] Building %s near chokepoint %d at (%d,%d)", unitID, toBuild.getName().c_str(), chokepointNo, start.x(), start.y());
		}
	}

	if (toBuild.isResourceDepot()) {
		//CommandCenter or Nexus. Build near 2nd mineral patch.
		bool found = false;
		TilePosition home = Broodwar->self()->getStartLocation();

		double bestDist = -1;
		Unit* mineral = NULL;

		for(set<Unit*>::iterator m = Broodwar->getMinerals().begin(); m != Broodwar->getMinerals().end(); m++) {
			double dist = (*m)->getDistance(Position(home));
			if (dist >= 500) {
				if (bestDist < 0 || dist < bestDist) {
					if (ExplorationManager::canReach(home, (*m)->getTilePosition())) {
						mineral = (*m);
						bestDist = dist;
						found = true;
					}
				}
			}
		}
		if (found) {
			start = mineral->getTilePosition();
		}
		else {
			//Check exploration manager
			SpottedObject* obj = ExplorationManager::getInstance()->findSpottedMineralPatch();
			if (obj != NULL) {
				start = obj->getTilePosition();
			}
		}
	}

	return start;
}

bool WorkerAgent::buildSpotExplored() {
	int sightDist = 64;
	if (toBuild.isRefinery()) {
		sightDist = 160; //5 tiles
	}

	double dist = unit->getPosition().getDistance(Position(buildSpot));
	//Broodwar->printf("Dist=%d, toReach=%d", (int)dist, sightDist);
	if (dist > sightDist) {
		//Broodwar->printf("Not there");
		return false;
	}
	//Broodwar->printf("Arrived");
	return true;
}

void WorkerAgent::setState(int state) {
	currentState = state;
	stringstream out;
	out << " State=";
	out << getStateAsText();
	info = out.str();
#ifdef _DEBUG
	Broodwar->printf("WorkerAgent [%d] setState %s", unitID, getStateAsText().c_str());
#endif
}

string WorkerAgent::getTypeName() {
	return "WorkerAgent";
}

bool WorkerAgent::assignToRepair(Unit* building) {
	if (unit->isIdle() || (unit->isGatheringMinerals() && !unit->isCarryingMinerals())) {
		unit->stop();
		setState(REPAIRING);
		unit->repair(building);
		return true;
	}
	return false;
}

bool WorkerAgent::assignToFinishBuild(Unit* building) {
	if (unit->isIdle() || (unit->isGatheringMinerals() && !unit->isCarryingMinerals())) {
		unit->stop();
		setState(REPAIRING);
		unit->rightClick(building);
		return true;
	}
	return false;
}

bool WorkerAgent::canBuild(UnitType type) {
	if (unit->isIdle() || (unit->isGatheringMinerals() && !unit->isCarryingMinerals())) {
		if (Broodwar->canMake(unit, type)) {
			return true;
		}
	}
	return false;
}

void WorkerAgent::assignToBuild(UnitType type) {
	unit->stop();
	setState(FIND_BUILDSPOT);
	toBuild = type;
	//Broodwar->printf("[%d] assigned to build %s", unitID, type.getName().c_str());
}

void WorkerAgent::reset() {
	if (currentState == MOVE_TO_SPOT) {
		//The buildSpot is probably not reachable. Block it.	
		CoverMap::getInstance()->blockPosition(buildSpot);
	}

	if (unit->isConstructing()) {
		unit->cancelConstruction();
	}

	setState(GATHER_MINERALS);
	unit->stop();
	BaseAgent* base = AgentManager::getInstance()->getClosestBase(unit->getTilePosition());
	if (base != NULL) {
		unit->rightClick(base->getUnit()->getPosition());
	}
	
	//Broodwar->printf("[%d] worker reset", unitID);
}

bool WorkerAgent::isConstructing(UnitType type) {
	if (currentState == FIND_BUILDSPOT || currentState == MOVE_TO_SPOT || currentState == CONSTRUCT) {
		if (toBuild.getID() == type.getID()) {
			return true;
		}
	}
	return false;
}

/** Returns the state of the agent as text. Good for printouts. */
string WorkerAgent::getStateAsText()
{
	string strReturn;
	switch(currentState)
	{
	case GATHER_MINERALS:
		strReturn = "GATHER_MINERALS";
		break;
	case GATHER_GAS:
		strReturn = "GATHER_GAS";
		break;
	case FIND_BUILDSPOT:
		strReturn = "FIND_BUILDSPOT";
		break;
	case MOVE_TO_SPOT:
		strReturn = "MOVE_TO_SPOT";
		break;
	case CONSTRUCT:
		strReturn = "CONSTRUCT";
		break;
	case REPAIRING:
		strReturn = "REPAIRING";
		break;
	};
	return strReturn;
}
