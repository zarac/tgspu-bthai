#include "ExplorationManager.h"
#include "AgentManager.h"
#include "VultureAgent.h"
#include "ComsatAgent.h"
#include "BaseAgent.h"
#include "Commander.h"
#include "Squad.h"

bool ExplorationManager::instanceFlag = false;
ExplorationManager* ExplorationManager::instance = NULL;

ExplorationManager::ExplorationManager() {
	lastUpdateFrame = 0;
	active = true;
	
	w = Broodwar->mapWidth();
	h = Broodwar->mapHeight();

	explore_map = new int*[w];
	for(int i = 0 ; i < w ; i++) {
		explore_map[i] = new int[h];

		for (int j = 0; j < h; j++) {
			explore_map[i][j] = 0;
		}
	}
}

ExplorationManager::~ExplorationManager() {
	for(int i = 0 ; i < w ; i++) {
		delete[] explore_map[i];
	}
	delete[] explore_map;

	for (int i = 0; i < (int)spottedBuildings.size(); i++) {
		//delete spottedBuildings.at(i);
	}
	for (int i = 0; i < (int)spottedResources.size(); i++) {
		//delete spottedResources.at(i);
	}

	instanceFlag = false;
	delete instance;
}

void ExplorationManager::setInactive() {
	active = false;
}

bool ExplorationManager::isActive() {
	return active;
}

void ExplorationManager::updateExploreMap() {
	lastUpdateFrame = Broodwar->getFrameCount();

	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			//Increase value by 1
			explore_map[x][y]++;

			if (!Broodwar->isBuildable(x, y)) {
				explore_map[x][y] = 0;
			}
		}
	}

	updateSeen();
}

void ExplorationManager::updateSeen() {
	vector<BaseAgent*> agents = AgentManager::getInstance()->getAgents();
	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->isAlive()) {
			TilePosition curPos = agents.at(i)->getUnit()->getTilePosition();
			for (int x = curPos.x() - 4; x <= curPos.x() + 4; x++) {
				for (int y = curPos.y() - 4; y <= curPos.y() + 4; y++) {
					if (x >= 0 && x < w && y >= 0 && y < h) {
						explore_map[x][y] = 0;
					}
				}
			}
		}
	}

	for (int j = 0; j < (int)spottedBuildings.size(); j++) {
		TilePosition oPos = spottedBuildings.at(j)->getTilePosition();
		if (explore_map[oPos.x()][oPos.y()] == 0) {
			//This tile is seen, update spotted objects if needed
			checkObject(spottedBuildings.at(j));
		}
	}
}

void ExplorationManager::checkObject(SpottedObject* obj) {
	TilePosition pos = obj->getTilePosition();
	set<Unit*> units = Broodwar->unitsOnTile(pos.x(), pos.y());

	//See if the spottedbuilding still exists
	bool found = false;
	for(set<Unit*>::const_iterator i=units.begin();i!=units.end();i++) {
		if ((*i)->getType().getID() == obj->getType().getID()) {
			found = true;
		}
		if ((*i)->getID() == obj->getUnitID()) {
			found = true;
		}
	}

	if (!found) {
		obj->setInactive();
	}
}

ExplorationManager* ExplorationManager::getInstance() {
	if (!instanceFlag) {
		instance = new ExplorationManager();
		instanceFlag = true;
	}
	return instance;
}

void ExplorationManager::computeActions() {
	if (!active) {
		return;
	}

	if (Broodwar->getFrameCount() % 200 == 0) {
		calcEnemyForceData();
		calcOwnForceData();
	}

	if (Broodwar->getFrameCount() - lastUpdateFrame >= 250) {
		updateExploreMap();
	}

	vector<Squad*> squads = Commander::getInstance()->getSquads();
	for (int i = 0; i < (int)squads.size(); i++) {
		if (squads.at(i)->isExplorer()) {
			handleSquad(squads.at(i));
		}
	}
}

void ExplorationManager::handleSquad(Squad* squad) {
	vector<BaseAgent*> agents = squad->getMembers();
	for (int i = 0; i < (int)agents.size(); i++) {
		handleAgent(agents.at(i));
	}
}

void ExplorationManager::handleAgent(BaseAgent* agent) {
	TilePosition curPos = agent->getUnit()->getTilePosition();
	TilePosition goal = getNextToExplore(curPos, agent);
	if (goal.x() >= 0) {
		agent->setGoal(goal);
		//Broodwar->printf("[%d@%d,%d] send to (%d,%d)", agent->getUnitID(), agent->getUnit()->getTilePosition().x(), agent->getUnit()->getTilePosition().y(), goal.x(), goal.y());
	}
}

TilePosition ExplorationManager::getNextToExplore(TilePosition start, BaseAgent* agent) {
	double bestVal = -1;
	int bestX = -1;
	int bestY = -1;
	double highVal = -100000;

	int startX = start.x();
	int startY = start.y();
	int range = 35;

	if (agent->getUnit()->isIdle()) {
		//The unit is idle = it has reached its goal, or is stuck.
		explore_map[agent->getGoal().x()][agent->getGoal().y()] = 0;
	}

	for (int x = startX - range; x <= startX + range; x++) {
		for (int y = startY - range; y <= startY + range; y++) {

			int dX = startX - x;
			if (dX < 0) dX = -dX;
			int dY = startY - y;
			if (dY < 0) dY = -dY;

			if (x >= 0 && x < w && y >= 0 && y < h && dX > 4 && dY > 4) {
				TilePosition cPos = TilePosition(x, y);
				double dist = start.getDistance(cPos);
				double explVal = (double)explore_map[x][y];

				//Check if we can reach the position
				bool ok = true;
				if (!agent->getUnit()->getType().isFlyer()) {
					if (!isWalkable(agent->getUnit()->getTilePosition(), cPos)) {
						ok = false;
					}
				}

				if (ok) {
					double cVal = explVal - dist / (double)100;
					if (explVal > 0 && cVal < 1) {
						//Make sure cVal is at least 1 if explorevalue is not 0.
						cVal = 1;
					}
					if (cVal >= bestVal) {
						bestVal = cVal;
						bestX = x;
						bestY = y;
					}
					if (cVal > highVal) {
						highVal = cVal;
					}
				}
			}
		}
	}

	//Broodwar->drawLine(CoordinateType::Map,start.x(),start.y(),bestX,bestY,Colors::Green);

	/*if (bestX < 0 || bestY < 0) {
		Broodwar->printf("Didnt find a move: %d", (int)highVal);
	}*/
	return TilePosition(bestX, bestY);
}

bool ExplorationManager::isWalkable(TilePosition start, TilePosition pos) {
	int startX = pos.x();
	int startY = pos.y();
	
	for (int x = startX - 1; x <= startX + 1; x++) {
		for (int y = startY - 1; y <= startY + 1; y++) {
			if (x >= 0 && x < w && y >= 0 && y < h) {
				if (!Broodwar->isBuildable(x, y)) {
					return false;
				}
			}
		}
	}

	if (!canReach(start, pos)) {
		return false;
	}

	return true;
}

void ExplorationManager::calcOwnForceData() {
	ownForce.airAttackStr = 0;
	ownForce.airDefendStr = 0;
	ownForce.groundAttackStr = 0;
	ownForce.groundDefendStr = 0;

	vector<BaseAgent*> agents = AgentManager::getInstance()->getAgents();
	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->isAlive()) {
			UnitType type = agents.at(i)->getUnit()->getType();
			if (type.canAttack() && !type.isWorker()) {
				if (!type.isBuilding()) {
					if (type.isFlyer()) {
						ownForce.airAttackStr += type.destroyScore();
					}
					else {
						ownForce.groundAttackStr += type.destroyScore();
					}
				}

				if (UnitAgent::getAirRange(type) >= 0) {
					ownForce.airDefendStr += type.destroyScore();
				}
				if (UnitAgent::getGroundRange(type) >= 0) {
					ownForce.groundDefendStr += type.destroyScore();
				}
			}
		}
	}
}

void ExplorationManager::calcEnemyForceData() {
	enemyForce.airAttackStr = 0;
	enemyForce.airDefendStr = 0;
	enemyForce.groundAttackStr = 0;
	enemyForce.groundDefendStr = 0;

	for (int i = 0; i < (int)spottedUnits.size(); i++) {
		if (spottedUnits.at(i)->isActive()) {
			UnitType type = spottedUnits.at(i)->getType();
			if (type.canAttack() && !type.isWorker()) {
				if (!type.isBuilding()) {
					if (type.isFlyer()) {
						enemyForce.airAttackStr += type.destroyScore();
					}
					else {
						enemyForce.groundAttackStr += type.destroyScore();
					}
				}

				if (UnitAgent::getAirRange(type) >= 0) {
					enemyForce.airDefendStr += type.destroyScore();
				}
				if (UnitAgent::getGroundRange(type) >= 0) {
					enemyForce.groundDefendStr += type.destroyScore();
				}
			}
		}
	}
}

void ExplorationManager::debug() {
	for (int i = 0; i < (int)spottedBuildings.size(); i++) {
		if (spottedBuildings.at(i)->isActive()) {
			int x1 = spottedBuildings.at(i)->getTilePosition().x() * 32;
			int y1 = spottedBuildings.at(i)->getTilePosition().y() * 32;
			int x2 = x1 + 32;
			int y2 = y1 + 32;

			Broodwar->drawBox(CoordinateType::Map,x1,y1,x2,y2,Colors::Blue,true);
		}
	}

	if (Broodwar->getFrameCount() % 250 == 0) {
		Broodwar->printf("[E] GroundAttackStr: %d, GroundDefStr: %d, AirAttackStr: %d, AirDefStr: %d", enemyForce.groundAttackStr, enemyForce.groundDefendStr, enemyForce.airAttackStr, enemyForce.airDefendStr);
		Broodwar->printf("[O] GroundAttackStr: %d, GroundDefStr: %d, AirAttackStr: %d, AirDefStr: %d", ownForce.groundAttackStr, ownForce.groundDefendStr, ownForce.airAttackStr, ownForce.airDefendStr);
	}
}

void ExplorationManager::addSpottedStartLocation(TilePosition pos) {
	SpottedObject* obj = new SpottedObject(Position(pos));
	spottedBuildings.push_back(obj);
}

void ExplorationManager::addSpottedUnit(Unit* unit) {
	if (unit->getType().isBuilding()) {
		bool found = false;
		for (int i = 0; i < (int)spottedBuildings.size(); i++) {
			if (spottedBuildings.at(i)->getUnitID() == unit->getID()) {
				found = true;
				break;
			}
		}

		if (!found) {
			//Broodwar->printf("[EM] Enemy %s spotted at (%d,%d)", unit->getType().getName().c_str(), unit->getPosition().x(), unit->getPosition().y());
			spottedBuildings.push_back(new SpottedObject(unit));
			BaseAgent::doScannerSweep(unit->getTilePosition());
		}
	}
	else {
		bool found = false;
		for (int i = 0; i < (int)spottedUnits.size(); i++) {
			if (spottedUnits.at(i)->getUnitID() == unit->getID()) {
				found = true;
				break;
			}
		}

		if (!found) {
			spottedUnits.push_back(new SpottedObject(unit));
		}
	}
}

void ExplorationManager::addSpottedResource(Unit* unit) {
	UnitType type = unit->getType();
	if (type.getID() == UnitTypes::Resource_Mineral_Field.getID() || type.getID() == UnitTypes::Resource_Vespene_Geyser.getID()) {
		bool found = false;
		for (int i = 0; i < (int)spottedResources.size(); i++) {
			if (spottedResources.at(i)->getUnitID() == unit->getID()) {
				found = true;
				break;
			}
		}

		if (!found) {
			//Broodwar->printf("[EM] %s spotted at (%d,%d)", type.getName().c_str(), unit->getPosition().x(), unit->getPosition().y());
			spottedResources.push_back(new SpottedObject(unit));
		}
	}
}

SpottedObject* ExplorationManager::findSpottedMineralPatch() {
	Position start = Position(Broodwar->self()->getStartLocation());

	double bestDist = -1;
	SpottedObject* bestObj = NULL;

	for (int i = 0; i < (int)spottedResources.size(); i++) {
		SpottedObject* obj = spottedResources.at(i);
		if (BaseAgent::isOfType(obj->getType(), UnitTypes::Resource_Mineral_Field)) {
			double dist = start.getDistance(obj->getPosition());
			if (dist >= 500) {
				if (BWTA::getGroundDistance(Broodwar->self()->getStartLocation(), obj->getTilePosition()) > 0) {
					if (bestDist < 0 || dist < bestDist) {
						bestDist = dist;
						bestObj = obj;
					}
				}
			}
		} 
	}

	return bestObj;
}

SpottedObject* ExplorationManager::findSpottedGeyser() {
	Position start = Position(Broodwar->self()->getStartLocation());

	double bestDist = -1;
	SpottedObject* foundObj = NULL;
	
	for (int i = 0; i < (int)spottedResources.size(); i++) {
		SpottedObject* obj = spottedResources.at(i);
		
		if (BaseAgent::isOfType(obj->getType(), UnitTypes::Resource_Vespene_Geyser)) {
			set<Unit*> unitsAtTile = Broodwar->unitsOnTile(obj->getTilePosition().x(), obj->getTilePosition().y());
			if ((int)unitsAtTile.size() == 0) {
				double cDist = obj->getDistance(start);
				if (bestDist < 0 || cDist < bestDist) {
					bestDist = cDist;
					foundObj = obj;
				}
			}
		}
	}

	return foundObj;
}

void ExplorationManager::unitDestroyed(Unit* unit) {
	if (unit->getType().isBuilding()) {
		//Broodwar->printf("[EM]: Remove %s", unit->getType().getName().c_str());
		TilePosition uPos = unit->getTilePosition();
		for (int i = 0; i < (int)spottedBuildings.size(); i++) {
			if (spottedBuildings.at(i)->getUnitID() == unit->getID()) {
				spottedBuildings.at(i)->setInactive();
			}
			if (spottedBuildings.at(i)->isAt(uPos)) {
				checkObject(spottedBuildings.at(i));
			}
		}
	}
	else {
		for (int i = 0; i < (int)spottedUnits.size(); i++) {
			if (spottedUnits.at(i)->getUnitID() == unit->getID()) {
				spottedUnits.at(i)->setInactive();
			}
		}
	}
}

vector<SpottedObject*> ExplorationManager::getSpottedBuildings() {
	return spottedBuildings;
}

bool ExplorationManager::canReach(TilePosition a, TilePosition b) {
	int w = Broodwar->mapWidth();
	int h = Broodwar->mapHeight();
	if (a.x() < 0 || a.x() >= w || a.y() < 0 || a.y() >= h) {
		Broodwar->printf("canReach() illegal position a(%d,%d)", a.x(), a.y());
	}
	if (b.x() < 0 || b.x() >= w || b.y() < 0 || b.y() >= h) {
		Broodwar->printf("canReach() illegal position b(%d,%d)", b.x(), b.y());
	}
	bool ok = true;

	//Broodwar->printf("Checking path (%d,%d) -> (%d,%d)", a.x(), a.y(), b.x(), b.y());
	if (analyzed) {
		BWTA::Region* regA = BWTA::getRegion(a);
		BWTA::Region* regB = BWTA::getRegion(b);
		if (regA != NULL && regB != NULL) {
			//Broodwar->printf("A (%d,%d) -> region center (%d,%d)", a.x(), a.y(), regA->getCenter().x(), regA->getCenter().y());
			//Broodwar->printf("B (%d,%d) -> region center (%d,%d)", b.x(), b.y(), regB->getCenter().x(), regB->getCenter().y());
			if (!regB->isReachable(regA) || !regA->isReachable(regB)) {
				//Broodwar->printf("[ExpMan]: (%d,%d)-(%d,%d) Cannot reach tile", a.x(), a.y(), b.x(), b.y());
				ok = false;
			}
		}
		else {
			//Broodwar->printf("[ExpMan]: (%d,%d)-(%d,%d) Cannot reach tile. Region unknown", a.x(), a.y(), b.x(), b.y());
			ok = false;
		}
	}
	
	return ok;
}

bool ExplorationManager::enemyIsProtoss() {
	for(set<Player*>::const_iterator i=Broodwar->getPlayers().begin();i!=Broodwar->getPlayers().end();i++) {
		if ((*i)->isEnemy(Broodwar->self())) {
			if ((*i)->getRace().getID() == Races::Protoss.getID()) {
				return true;
			}
		}
	}
	return false;
}

bool ExplorationManager::enemyIsZerg() {
	for(set<Player*>::const_iterator i=Broodwar->getPlayers().begin();i!=Broodwar->getPlayers().end();i++) {
		if ((*i)->isEnemy(Broodwar->self())) {
			if ((*i)->getRace().getID() == Races::Zerg.getID()) {
				return true;
			}
		}
	}
	return false;
}

bool ExplorationManager::enemyIsTerran() {
	for(set<Player*>::const_iterator i=Broodwar->getPlayers().begin();i!=Broodwar->getPlayers().end();i++) {
		if ((*i)->isEnemy(Broodwar->self())) {
			if ((*i)->getRace().getID() == Races::Terran.getID()) {
				return true;
			}
		}
	}
	return false;
}

bool ExplorationManager::enemyIsUnknown() {
	if (!enemyIsTerran() && !enemyIsProtoss() && !enemyIsZerg()) {
		return true;
	}
	return false;
}

int ExplorationManager::calcMineralsAroundPosition(TilePosition pos) {
	int totMinerals = 0;

	int maxDist = 20 * 32;
	for(set<Unit*>::iterator m = Broodwar->getMinerals().begin(); m != Broodwar->getMinerals().end(); m++) {
		double dist = (*m)->getDistance(Position(pos));
		if (dist <= maxDist) {
			totMinerals += (*m)->getResources();
		}
	}

	return totMinerals;
}
