#include "Squad.h"
#include "UnitAgent.h"

Squad::Squad(int mId, int mType, string mName) {
	this->id = mId;
	this->type = mType;
	this->name = mName;
	active = false;
	goal = TilePosition(-1, -1);
}

int Squad::getID() {
	return id;
}

bool Squad::isActive() {
	return active;
}

void Squad::addSetup(UnitType type, int no) {
	UnitSetup us;
	us.type = type;
	us.no = no;
	us.current = 0;
	setup.push_back(us);
}

vector<UnitSetup> Squad::getSetup() {
	return setup;
}

void Squad::computeActions() {
	
}

bool Squad::isUnderAttack() {
	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->isUnderAttack()) {
			return true;
		}
	}
	return false;
}

bool Squad::needUnit(UnitType type) {
	for (int i = 0; i < (int)setup.size(); i++) {
		if (setup.at(i).equals(type)) {
			//Found a matching setup, see if there is room
			if (setup.at(i).current < setup.at(i).no) {
				return true;
			}
		}
	}
	return false;
}

vector<BaseAgent*> Squad::getMembers() {
	return agents;
}

bool Squad::addMember(BaseAgent* agent) {
	//Step 1. Check if the agent already is in the squad
	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->getUnitID() == agent->getUnitID()) {
			return false;
		}
	}

	//Step 2. Check if we have room for this type of agent.
	for (int i = 0; i < (int)setup.size(); i++) {
		if (setup.at(i).equals(agent->getUnit()->getType())) {
			//Found a matching setup, see if there is room
			if (setup.at(i).current < setup.at(i).no) {
				//Yes we have, add it to the squad
				agents.push_back(agent);
				agent->setSquadID(id);
				setMemberState(agent);
				setup.at(i).current++;

				if (goal.x() >= 0) {
					agent->setGoal(goal);
				}

				if (isFull()) {
					active = true;
					printInfo();
				}

				return true;
			}
		}
	}

	return false;
}

void Squad::setMemberState(BaseAgent* agent) {
	UnitAgent* uAgent = (UnitAgent*)agent;
	if (isOffensive()) {
		uAgent->assignToAttack();
	}
	else if (isExplorer()) {
		uAgent->assignToExplore();
	}
	else {
		uAgent->assignToDefend();
	}
}

void Squad::printInfo() {
	string f = "NotFull";
	if (isFull()) {
		f = "Full";
	}
	string a = "Inactive";
	if (isActive()) {
		a = "Active";
	}

	Broodwar->printf("[SQ %d] %s (%s, %s)", id, name.c_str(), f.c_str(), a.c_str());
}

void Squad::printFullInfo() {
	printInfo();

	for (int i = 0; i < (int)setup.size(); i++) {
		Broodwar->printf("%s, %d/%d", setup.at(i).type.getName().c_str(), setup.at(i).current, setup.at(i).no);
	}
}

bool Squad::isFull() {
	for (int i = 0; i < (int)setup.size(); i++) {
		if (setup.at(i).current < setup.at(i).no) {
			return false;
		}
	}
	return true;
}

void Squad::removeMember(BaseAgent* agent) {
	//Step 1. Remove the agent instance
	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->getUnitID() == agent->getUnitID()) {
			agents.at(i)->setSquadID(-1);
			agents.at(i)->assignToDefend();
			agents.erase(agents.begin() + i);
			//Broodwar->printf("Removed %s from Squad %d", agent->getUnit()->getType().getName().c_str(), id);
			break;
		}
	}

	//Special case: Siege Tanks
	UnitType type = agent->getUnit()->getType();
	if (type.getID() == UnitTypes::Terran_Siege_Tank_Siege_Mode.getID()) {
		type = UnitTypes::Terran_Siege_Tank_Tank_Mode;
	}

	//Step 2. Update the setup list
	for (int i = 0; i < (int)setup.size(); i++) {
		if (setup.at(i).equals(type)) {
			//Broodwar->printf("Decreased %s to %d/%d", setup.at(i).type.getName().c_str(), setup.at(i).current, setup.at(i).no);
			setup.at(i).current--;
		}
	}
}

BaseAgent* Squad::removeMember(UnitType type) {
	BaseAgent* agent = NULL;

	//Special case: Siege Tanks
	if (type.getID() == UnitTypes::Terran_Siege_Tank_Siege_Mode.getID()) {
		type = UnitTypes::Terran_Siege_Tank_Tank_Mode;
	}

	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->isOfType(type)) {
			agent = agents.at(i);
			break;
		}
	}

	if (agent != NULL) {
		removeMember(agent);
	}

	return agent;
}

bool Squad::isGathered() {
	//If we only have 1 unit in the squad...
	if (getSize() == 1) {
		if (isFull()) {
			return true;
		}
	}

	if (getSize() > 0) {
		TilePosition center = getCenter();
		int tot = 0;
		int within = 0;
		int maxRange = 4*32;

		for (int i = 1; i < (int)agents.size(); i++) {
			if (agents.at(i)->isAlive()) {
				double dist = agents.at(i)->getUnit()->getDistance(Position(center));
				if (dist <= maxRange) {
					within++;
				}
				tot++;
			}
		}

		int pct = within * 100 / tot;
		if (pct >= 90) {
			return true;
		}
	}

	return false;
}

void Squad::setGoal(TilePosition mGoal) {
	if (mGoal.x() != goal.x() || mGoal.y() != goal.y()) {
		this->goal = mGoal;
		setMemberGoals();
	}
}

void Squad::clearGoal() {
	this->goal = TilePosition(-1, -1);
	setMemberGoals();
}

void Squad::setMemberGoals() {
	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->isAlive()) {
			agents.at(i)->setGoal(goal);
		}
	}
}

TilePosition Squad::getGoal() {
	return goal;
}

bool Squad::hasGoal() {
	if (goal.x() < 0 || goal.y() < 0) {
		return false;
	}
	return true;
}

bool Squad::isThisGoal(TilePosition mGoal) {
	int xDiff = goal.x() - mGoal.x();
	if (xDiff < 0) xDiff *= -1;
	int yDiff = goal.y() - mGoal.y();
	if (yDiff < 0) yDiff *= -1;

	//Broodwar->printf("SQ %d: (%d,%d) = (%d,%d)", id, goal.x(), goal.y(), mGoal.x(), mGoal.y());
	if (xDiff <= 2 && yDiff <= 2) {
		return true;
	}
	return false;
}

bool Squad::isCloseTo(TilePosition mGoal) {
	double dist = mGoal.getDistance(goal);
	if (dist <= 3) {
		return true;
	}
	return false;
}

TilePosition Squad::getCenter() {
	int cX = 0;
	int cY = 0;
	int cnt = 0;

	for (int i = 1; i < (int)agents.size(); i++) {
		if (agents.at(i)->isAlive()) {
			cX += agents.at(i)->getUnit()->getTilePosition().x();
			cY += agents.at(i)->getUnit()->getTilePosition().y();
			cnt++;
		}
	}

	// They might be all dead.
	if(cnt > 0)
	{
		cX = cX / cnt;
		cY = cY / cnt;
	}

	return TilePosition(cX, cY);
}

int Squad::getSize() {
	return (int)agents.size();
}

int Squad::getStrength() {
	int str = 0;

	for (int i = 1; i < (int)agents.size(); i++) {
		if (agents.at(i)->isAlive()) {
			str += agents.at(i)->getUnit()->getType().destroyScore();
		}
	}

	return str;
}

bool Squad::isOffensive() {
	return type == OFFENSIVE;
}

bool Squad::isDefensive() {
	return type == DEFENSIVE;
}

bool Squad::isExplorer() {
	return type == EXPLORER;
}

bool Squad::isSupport() {
	return type == SUPPORT;
}

bool Squad::isBunkerDefend() {
	return type == BUNKER;
}

void Squad::disband(TilePosition retreatPoint) {
	for (int i = 1; i < (int)agents.size(); i++) {
		if (agents.at(i)->isAlive()) {
			agents.at(i)->setGoal(retreatPoint);
		}
	}
	active = false;
}

int Squad::getHealthPct() {
	int need = 0;
	int current = 0;

	for (int i = 0; i < (int)setup.size(); i++) {
		need += setup.at(i).no;
		current += setup.at(i).current;
	}

	return (int)(current * 100 / need);
}

bool Squad::canMerge(Squad* squad) {
	//Step 1. Check so the two squads are different
	if (squad->getID() == getID()) {
		return false;
	}

	//Step 2. If the other squad is full, we dont
	//use it to merge with.
	if (squad->isFull()) {
		return false;
	}

	if (getHealthPct() < squad->getHealthPct()) {
		//Can only merge with smaller squads
		return false;
	}

	//Step 3. Check if we can merge
	bool ok = false;
	for (int i = 0; i < (int)setup.size(); i++) {
		if (setup.at(i).current < setup.at(i).no) {
			//I need this number of units of the specified type
			int need = setup.at(i).no - setup.at(i).current;
			if (squad->hasUnits(setup.at(i).type, need)) {
				//The other squad have something we need, merge
				ok = true;
			}
		}
	}
	if (!ok) {
		return false;
	}

	//Step 4. Do the merge
	for (int i = 0; i < (int)setup.size(); i++) {
		if (setup.at(i).current < setup.at(i).no) {
			int need = setup.at(i).no - setup.at(i).current;
			for (int j = 0; j < need; j++) {
				BaseAgent* agent = squad->removeMember(setup.at(i).type);
				if (agent != NULL) {
					if (!agent->isAlive() || !agent->getUnit()->exists()) {
						Broodwar->printf("[%d] WARNING merge with destroyed agent %s", id, agent->getUnit()->getType().getName().c_str());
					}
					addMember(agent);
				}
			}
		}
	}

	return true;
}

bool Squad::hasUnits(UnitType type, int no) {
	//Special case: Siege Tanks
	if (type.getID() == UnitTypes::Terran_Siege_Tank_Siege_Mode.getID()) {
		type = UnitTypes::Terran_Siege_Tank_Tank_Mode;
	}

	for (int i = 0; i < (int)setup.size(); i++) {
		if (setup.at(i).equals(type)) {
			if (setup.at(i).current >= no) {
				//I have these units
				return true;
			}
		}
	}
	return false;
}
