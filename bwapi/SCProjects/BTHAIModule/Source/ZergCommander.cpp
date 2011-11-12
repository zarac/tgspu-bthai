#include "ZergCommander.h"
#include "AgentManager.h"
#include "ExplorationManager.h"

ZergCommander::ZergCommander() {
	level = 0;
	idealWorkerCount = 1;
	workersNeeded = 0;
	currentState = DEFEND;
	currentID = 1;
	hydraSquads = 0;

	addMainAttackSquad();
	addHydraliskSquad();

	//sixPool = new Squad(currentID, Squad::DEFENSIVE, "Six-Pool");
	//sixPool->addSetup(UnitTypes::Zerg_Zergling, 6);
	//squads.push_back(sixPool);
	//currentID++;
}

void ZergCommander::addMainAttackSquad() {
	Squad* squad = new Squad(currentID, Squad::OFFENSIVE, "MainAttackSquad");
	squad->addSetup(UnitTypes::Zerg_Zergling, 2);
	squad->addSetup(UnitTypes::Zerg_Hydralisk, 10);
	squads.push_back(squad);
	currentID++;
}

void ZergCommander::addHydraliskSquad()
{
	Broodwar->printf("Adding a hydralisk squad.");
	Squad* squad = new Squad(currentID, Squad::OFFENSIVE, "HydraliskSquad");
	squad->addSetup(UnitTypes::Zerg_Hydralisk, 24);
	squads.push_back(squad);
	currentID++;
	hydraSquads++;
}

ZergCommander::~ZergCommander() {
	for (int i = 0; i < (int)squads.size(); i++) {
		delete squads.at(i);
	}
	delete instance;
}

void ZergCommander::computeActions() {

	//map<string, int> agents;
	AgentManager* agentManager = AgentManager::getInstance();
	vector<BaseAgent*> agents = agentManager->getAgents();
	vector<BaseAgent*>::iterator agent = agents.begin();
	vector<BaseAgent*>::iterator agentsend = agents.end();
	while (agent != agentsend)
	{
		if ((*agent)->getTypeName() == "HatcheryAgent")
			Broodwar->printf("agent type = %i", (*agent)->getUnit()->getType());
		agent++;
	}
	// Why doesn't this work? (crash)
	//for (vector<BaseAgent*>::iterator agent = agentManager->getAgents().begin(); agent != agentManager->getAgents().end(); agent++)
	//{
	//}

	updateWorkersNeeded();
	manageLevel();

	if (currentState == DEFEND) {
		int noOffSquads = 0;

		for (int i = 0; i < (int)squads.size(); i++) {
			if (squads.at(i)->isFull() && squads.at(i)->isOffensive()) {
				noOffSquads++;
			}
		}

		//Time to attack
		//TODO: Currently attacks when we have 2 full squads. Change if needed.
		if (noOffSquads >= 2) {
			currentState = ATTACK;
		}
	}

	if (currentState == DEFEND) {
		for (int i = 0; i < (int)squads.size(); i++) {
			if (!squads.at(i)->hasGoal()) {
				BWTA::Chokepoint* choke = getNextChokePoint();
				if (choke != NULL) {
					//Broodwar->printf("Assigned SQ %d to chokepoint %d", squads.at(i)->getID(), chokepointNo);
					TilePosition defSpot = TilePosition(choke->getCenter());
					squads.at(i)->setGoal(defSpot);
				}
			}
		}
	}

	TilePosition closest = getClosestEnemyBuilding(Broodwar->self()->getStartLocation());
	if (currentState == ATTACK) {
		for (int i = 0; i < (int)squads.size(); i++) {
			if (squads.at(i)->isActive() && squads.at(i)->isOffensive()) {
				if (closest.x() >= 0) {
					squads.at(i)->setGoal(closest);
				}
			}
			else {
				BWTA::Chokepoint* choke = getNextChokePoint();
				if (choke != NULL) {
					TilePosition defSpot = TilePosition(choke->getCenter());
					squads.at(i)->setGoal(defSpot);
				}
			}
		}
	}

	//TilePosition closest = getClosestEnemyBuilding(Broodwar->self()->getStartLocation());
	//sixPool->setGoal(closest);

	// adding hydralisk squad, 'cause it's great fun! : )
	if (Broodwar->self()->minerals() > 1000 &&
		Broodwar->self()->gas() > 1000
		&& hydraSquads < 5)
		addHydraliskSquad();
}

void ZergCommander::manageLevel()
{
	if (level == 0)
	{
		level = 1;
		Broodwar->printf("[ZergCommander] Reached level 1");
		idealWorkerCount = 8;
	}
	// TODO : check if we're training overlord, then we can count that as well
	else if (level == 1
		&& AgentManager::getInstance()->countNoUnits(UnitTypes::Zerg_Overlord) >= 2)
	{
		level = 2;
		idealWorkerCount = 15;
		Broodwar->printf("[ZergCommander] Reached level 2");
	}
	else if (level == 2 && 
		AgentManager::getInstance()->countNoUnits(UnitTypes::Zerg_Extractor) >= 1)
	{
		level = 3;
		idealWorkerCount = 20;
		Broodwar->printf("[ZergCommander] Reached level 3");
	}
	else if (level == 3
		&& AgentManager::getInstance()->countNoUnits(UnitTypes::Zerg_Hydralisk_Den) >= 1)
	{
		level = 4;
		idealWorkerCount = 30;
		Broodwar->printf("[ZergCommander] Reached level 4");
	}
	else if (level == 4
		&& AgentManager::getInstance()->countNoUnits(UnitTypes::Zerg_Sunken_Colony) >= 2)
	{
		level = 5;
		idealWorkerCount = 40;
		Broodwar->printf("[ZergCommander] Reached level 5");
	}
	else if (level == 5
		&& AgentManager::getInstance()->countNoUnits(UnitTypes::Zerg_Spire) >= 1)
	{
		level = 6;
		idealWorkerCount = 50;
		Broodwar->printf("[ZergCommander] Reached level 6");
	}
}

void ZergCommander::updateWorkersNeeded()
{
	workersNeeded = idealWorkerCount - AgentManager::getInstance()->getNoWorkers();
	if (workersNeeded > 0)
	{
		//. Check if there is a Worker already being built
		std::vector<BaseAgent*> agents = AgentManager::getInstance()->getAgents();
		for (unsigned int i = 0; i < agents.size(); i++)
		{
			BaseAgent* agent = agents.at(i);
			if (agent->isOfType(UnitTypes::Zerg_Egg)
				&& agent->getUnit()->getBuildType() == UnitTypes::Zerg_Drone)
					workersNeeded--;
		}
	}
}

int ZergCommander::getHydraliskSquadsCount()
{
	return hydraSquads;
}

int ZergCommander::getIdealWorkerCount()
{
	return idealWorkerCount;
}

int ZergCommander::getWorkersNeeded()
{
	return workersNeeded;
}

void ZergCommander::setWorkersNeeded(int p_workersNeeded)
{
	workersNeeded = p_workersNeeded;
}

int ZergCommander::getLevel()
{
	return level;
}

int ZergCommander::getState()
{
	return currentState;
}