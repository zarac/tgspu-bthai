#include "BaseAgent.h"
#include "BuildPlanner.h"
#include "AgentManager.h"
#include "ComsatAgent.h"

BaseAgent::BaseAgent() {
	alive = true;
	lastHitPoints = 0;
	info = "";
	squadID = -1;

	hLockEvent = CreateEvent( 
		NULL,         // no security attributes
		TRUE,         // manual-reset event
		TRUE,         // initial state is signaled
		NULL          // object name
	);
}

BaseAgent::BaseAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();

	BaseAgent();
}

BaseAgent::~BaseAgent() 
{
	for(int i = 0; i != eventCallbacks.size(); i++)
	{
		AgentRemovedEvent eventCallback = (eventCallbacks.at(i).eventCallback);
		eventCallback(this, eventCallbacks.at(i).ObjectToStore);
	}
	CloseHandle(hLockEvent);
}

BaseAgent* BaseAgent::getAgentReference(AgentRemovedEvent eventCallback, void* ObjectToStore)
{
	AGENTREMOVEDPACK AgentRemovedPack;
	AgentRemovedPack.eventCallback = eventCallback;
	AgentRemovedPack.ObjectToStore = ObjectToStore;
	eventCallbacks.insert(eventCallbacks.begin(), AgentRemovedPack);
	return this;
}

void BaseAgent::removeAgentReference(AgentRemovedEvent eventCallback, void* ObjectToStore)
{
	// hkl : rasmus fix (i think)
	for(unsigned int i = 0; i < eventCallbacks.size(); i++)
	{
		if(eventCallbacks.at(i).eventCallback == eventCallback && eventCallbacks.at(i).ObjectToStore == ObjectToStore)
		{
			eventCallbacks.erase(eventCallbacks.begin() + i);
			i--;
		}
	}
}

void BaseAgent::printInfo() {
	Broodwar->printf("[%d] (%s) %s, @%i,%i", unitID, getTypeName().c_str(), info.c_str(), unit->getPosition().x(), unit->getPosition().y());
}

void BaseAgent::setInfo(string mInfo) {
	this->info = mInfo;
}

int BaseAgent::getUnitID() {
	return unit->getID();
}

Unit* BaseAgent::getUnit() {
	return unit;
}

bool BaseAgent::matches(Unit *mUnit) {
	if (mUnit->getID() == unitID) {
		return true;
	}
	return false;
}

bool BaseAgent::isOfType(UnitType type) {
	if (unit->getType().getID() == type.getID()) {
		return true;
	}
	return false;
}

bool BaseAgent::isOfType(Unit* mUnit, UnitType type) {
	if (mUnit->getType().getID() == type.getID()) {
		return true;
	}
	return false;
}

bool BaseAgent::isOfType(UnitType mType, UnitType toCheckType) {
	if (mType.getID() == toCheckType.getID()) {
		return true;
	}
	return false;
}

bool BaseAgent::canBuild(UnitType type) {
	if (unit->isBeingConstructed()) {
		return false;
	}

	bool lock = BuildPlanner::getInstance()->isLocked();
	if (Broodwar->self()->minerals() - type.mineralPrice() > 300 && Broodwar->self()->gas() - type.gasPrice() > 200) {
		lock = false;
	}

	if (unit->isIdle() && !lock) {
		if (Broodwar->canMake(unit, type)) {
			return true;
		}
	}
	return false;
}

bool BaseAgent::isBuilding() {
	if (unit->getType().isBuilding()) {
		return true;
	}
	return false;
}

bool BaseAgent::isWorker() {
	if (unit->getType().isWorker()) {
		return true;
	}
	return false;
}

bool BaseAgent::isFreeWorker() {
	if (unit->getType().isWorker()) {
		if (unit->isIdle() || unit->isGatheringMinerals()) {
			return true;
		}
	}
	return false;
}

bool BaseAgent::isUnit() {
	if (unit->getType().isBuilding() || unit->getType().isWorker()) {
		return false;
	}
	return true;
}

bool BaseAgent::isUnderAttack() {
	bool attacked = false;
	int currentHitPoints = unit->getHitPoints();
	if (currentHitPoints < lastHitPoints) { // && !unit->isBeingConstructed()) {
		attacked = true;
	}
	lastHitPoints = currentHitPoints;
	return attacked;
}

void BaseAgent::destroyed() {
	alive = false;
}

bool BaseAgent::isAlive() {
	return alive;
}

bool BaseAgent::isDamaged() {
	if (unit->getHitPoints() < unit->getType().maxHitPoints()) {
		return true;
	}
	return false;
}

bool BaseAgent::isDetectorWithinRange(TilePosition pos, int range) {
	for(set<Unit*>::const_iterator i=Broodwar->enemy()->getUnits().begin();i!=Broodwar->enemy()->getUnits().end();i++) {
		if ((*i)->getType().isDetector()) {
			double dist = (*i)->getDistance(Position(pos));
			if (dist <= range) {
				return true;
			}
		}
	}
	return false;
}

bool BaseAgent::doScannerSweep(TilePosition pos) {
	if (!BuildPlanner::isTerran()) {
		return false;
	}

	BaseAgent* comsat = AgentManager::getInstance()->getAgent(UnitTypes::Terran_Comsat_Station);
	if (comsat != NULL) {
		return ((ComsatAgent*)comsat)->sweepArea(pos);
	}

	return false;
}

BWTA::Chokepoint* BaseAgent::getClosestChokePoint() {
	if (!analyzed) {
		return NULL;
	}

	//get the chokepoints linked to our home region
	std::set<BWTA::Chokepoint*> chokepoints= home->getChokepoints();
	double min_length=10000;
	BWTA::Chokepoint* choke=NULL;

	//iterate through all chokepoints and look for the one with the smallest gap (least width)
	for(std::set<BWTA::Chokepoint*>::iterator c=chokepoints.begin();c!=chokepoints.end();c++) {
		double length=(*c)->getWidth();
		if (length<min_length || choke==NULL) {
			min_length=length;
			choke=*c;
		}
	}
	return choke;
}

int BaseAgent::getNoChokePoints() {
	if (!analyzed) {
		return 1;
	}

	std::set<BWTA::Chokepoint*> chokepoints= home->getChokepoints();
	return (int)chokepoints.size();
}

BWTA::Chokepoint* BaseAgent::getChokePoint(int nr) {
	if (!analyzed) {
		return NULL;
	}

	std::set<BWTA::Chokepoint*> chokepoints= home->getChokepoints();
	BWTA::Chokepoint* choke=NULL;
	int cnt = 0;
	//iterate through all chokepoints and look for the one with the smallest gap (least width)
	for(std::set<BWTA::Chokepoint*>::iterator c=chokepoints.begin();c!=chokepoints.end();c++) {
		if (cnt == nr) {
			choke=*c;
			break;
		}
		cnt++;
	}
	return choke;
}

void BaseAgent::setSquadID(int id) {
	squadID = id;
}

int BaseAgent::getSquadID() {
	return squadID;
}

void BaseAgent::LockAgent()
{
	bBlock = false;
	if(WaitForSingleObject(hLockEvent, 1) == WAIT_TIMEOUT)
	{
		//bBlock = true;
		Broodwar->printf("WAIT_TIMEOUT");
	}
	else
	{
		//bBlock = false;
		//Broodwar->printf("OK!!!");
	}
	ResetEvent(hLockEvent);
}

void BaseAgent::UnLockAgent()
{
	if(bBlock)
		Broodwar->printf("Block");
	SetEvent(hLockEvent);
}
