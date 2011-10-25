#include "AgentManager.h"
#include "AgentFactory.h"
#include "CoverMap.h"
#include "Commander.h"

bool AgentManager::instanceFlag = false;
AgentManager* AgentManager::instance = NULL;

AgentManager::AgentManager() {

}

AgentManager::~AgentManager() {
	instanceFlag = false;
	for (int i = 0; i < (int)agents.size(); i++) {
		delete agents.at(i);
	}
	delete instance;
}

AgentManager* AgentManager::getInstance() {
	if (!instanceFlag) {
		instance = new AgentManager();
		instanceFlag = true;
	}
	return instance;
}

BaseAgent* AgentManager::at(int i)
{
	BaseAgent* agent = agents.at(i);
	agent->LockAgent();
	return agent;
}

vector<BaseAgent*> AgentManager::getAgents() {
	return agents;
}

void AgentManager::release(BaseAgent* agent)
{
	if(agent == NULL)
		return;
	agent->UnLockAgent();
}

int AgentManager::size()
{
	return agents.size();
}

BaseAgent* AgentManager::getAgent(int unitID) {
	BaseAgent* agent = NULL;

	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->getUnitID() == unitID) {
			agent = agents.at(i);
			break;
		}
	}

	return agent;
}

BaseAgent* AgentManager::getAgent(UnitType type) {
	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->isOfType(type) && agents.at(i)->isAlive()) {
			return agents.at(i);
		}
	}
	return NULL;
}

BaseAgent* AgentManager::getCombatAgent(UnitType type) {
	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->isOfType(type) && agents.at(i)->isAlive() && (agents.at(i)->isAttacking() || agents.at(i)->isDefending())) {
			return agents.at(i);
		}
	}
	return NULL;
}

BaseAgent* AgentManager::getClosestBase(TilePosition pos) {
	return getClosestAgent(pos, Broodwar->self()->getRace().getCenter());
}

BaseAgent* AgentManager::getClosestAgent(TilePosition pos, UnitType type) {
	BaseAgent* agent = NULL;
	double bestDist = -1;

	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->isOfType(type) && agents.at(i)->isAlive()) {
			double dist = agents.at(i)->getUnit()->getDistance(Position(pos));
			if (bestDist < 0 || dist < bestDist) {
				bestDist = dist;
				agent = agents.at(i);
			}
		}
	}
	return agent;
}

void AgentManager::addAgent(Unit* unit) {
	bool found = false;
	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->matches(unit)) {
			found = true;
			break;
		}
	}

	if (!found) {
		BaseAgent* newAgent = AgentFactory::getInstance()->createAgent(unit);
		agents.push_back(newAgent);

		if (newAgent->isBuilding()) {
			CoverMap::getInstance()->addConstructedBuilding(unit);
		}
		
		if (newAgent->isUnit()) {
			Commander::getInstance()->unitCreated(newAgent);
		}
	}
}

void AgentManager::addMorphAgent(Unit *unit)
{
	// Remove old agent.
	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->matches(unit)) {
			removeAgent(unit);
		}
	}

	// Create new... yeah!
	BaseAgent* newAgent = AgentFactory::getInstance()->createAgent(unit);
	agents.push_back(newAgent);

	if (newAgent->isBuilding()) {
		CoverMap::getInstance()->addConstructedBuilding(unit);
	}

	if (newAgent->isUnit()) {
		Commander::getInstance()->unitCreated(newAgent);
	}
}

void AgentManager::removeAgent(Unit* unit) {
	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->matches(unit)) {
			
			if (agents.at(i)->isBuilding()) {
				CoverMap::getInstance()->buildingDestroyed(unit);
			}

			agents.at(i)->destroyed();

			Commander::getInstance()->unitDestroyed(agents.at(i));

			return;
		}
	}
}

void AgentManager::cleanup() {
	//Step 1. Check if any agent is under attack. If so, dont cleanup since
	//it might cause a Nullpointer.
	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->isUnderAttack()) {
			return;
		}
	}

	//Step 2. Do the cleanup.
	int cnt = 0;
	int oldSize = (int)agents.size();
	for (int i = 0; i < (int)agents.size(); i++) {
		if (!agents.at(i)->isAlive()) {
			agents.at(i)->LockAgent();
			delete agents.at(i);
			agents.erase(agents.begin() + i);
			cnt++;
			i--;
		}
	}
	int newSize = (int)agents.size();
	if (cnt > 0) {
		Broodwar->printf("[AgentManager] removed %d inactive agents %d->%d", cnt, oldSize, newSize);
	}
}

void AgentManager::computeActions() {
	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->isAlive()) {
			agents.at(i)->computeActions();
		}
	}
}

int AgentManager::getNoWorkers() {
	int wCnt = 0;
	for (int i = 0; i < (int)agents.size(); i++) {
		BaseAgent* agent = agents.at(i);
		if (agent != NULL && agent->isWorker() && agent->isAlive()) {
			wCnt++;
		}
	}
	return wCnt;
}

BaseAgent* AgentManager::getWorkerAt(int i)
{
	int nWorkerPos = 0;
	for (int i = 0; i < (int)agents.size(); i++) {
		BaseAgent* agent = agents.at(i);
		if (agent != NULL && agent->isAlive() && agent->isWorker()) {
			if(nWorkerPos == i)
			{
				agent->LockAgent();
				return agent;
			}
			nWorkerPos++;
		}
	}
	return NULL;
}

BaseAgent* AgentManager::getFreeWorker()
{
	for (int i = 0; i < (int)agents.size(); i++) {
		BaseAgent* agent = agents.at(i);
		if (agent != NULL && agent->isAlive() && agent->isWorker()) {
			if (agent->getUnit()->isIdle() || (agent->getUnit()->isGatheringMinerals() && !agent->getUnit()->isCarryingMinerals())) {
				agent->LockAgent();
				return agent;
			}
		}
	}
	return NULL;
}

BaseAgent* AgentManager::findClosestFreeWorker(TilePosition pos) {
	BaseAgent* bestAgent = NULL;
	double bestDist = -1;

	for (int i = 0; i < (int)getNoWorkers(); i++) {
		BaseAgent* agent = getWorkerAt(i);
		if (agent != NULL && agent->isFreeWorker()) {
			double cDist = agent->getUnit()->getDistance(Position(pos));
			if (bestDist < 0 || cDist < bestDist) {
				bestDist = cDist;
				bestAgent = agent;
			}
		}
		release(agent);
	}
	return bestAgent;
}

bool AgentManager::isAnyAgentRepairingThisAgent(BaseAgent* repairedAgent)
{
	for (int i = 0; i < (int)agents.size(); i++) {
		BaseAgent* agent = agents.at(i);
		if (agent->isAlive() && agent->isWorker()) {
			Unit* unit = agent->getUnit();
			if (unit->isRepairing() || unit->isConstructing()) {
				if (unit->getTarget() != NULL && unit->getTarget()->getID() == repairedAgent->getUnitID()) {
					//Already have an assigned builder
					return true;
				}
			}
		}
	}
	return false;
}

int AgentManager::countNoUnits(UnitType type) {
	int cnt = 0;
	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->isAlive()) {
			if (agents.at(i)->isOfType(type)) {
				cnt++;
			}
		}
	}
	return cnt;
}

int AgentManager::countNoBases() {
	int cnt = 0;
	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->isAlive()) {
			if (agents.at(i)->getUnit()->getType().isResourceDepot()) {
				cnt++;
			}
		}
	}
	return cnt;
}

int AgentManager::countNoAttackingUnits() {
	int cnt = 0;
	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->isAlive()) {
			if (agents.at(i)->isAttacking()) {
				cnt++;
			}
		}
	}
	return cnt;
}

int AgentManager::countNoDefendingUnits() {
	int cnt = 0;
	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->isAlive()) {
			if (agents.at(i)->isDefending()) {
				cnt++;
			}
		}
	}
	return cnt;
}
