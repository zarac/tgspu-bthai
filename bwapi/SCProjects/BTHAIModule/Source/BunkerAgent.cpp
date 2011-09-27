#include "BunkerAgent.h"
#include "MarineAgent.h"
#include "FirebatAgent.h"
#include "ExplorationManager.h"
#include "AgentManager.h"
#include "Commander.h"
#include "Squad.h"

BunkerAgent::BunkerAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	Broodwar->printf("BunkerAgent created (%s)", unit->getType().getName().c_str());

	noMarines = 0;
	noFirebats = 0;
}

BunkerAgent::~BunkerAgent()
{
	// We don't want callbacks to a removed object!
	for(int i=0;i!=(int)loadedUnits.size();i++)
	{
		loadedUnits.at(i)->removeAgentReference(AgentRemovedEvent, this);
	}
}

void BunkerAgent::computeActions() {
	//Stim Pack trick
	if ((int)loadedUnits.size() > 0) {
		if (Broodwar->self()->hasResearched(TechTypes::Stim_Packs))  {
			for (int i = 0; i < (int)loadedUnits.size(); i++) {
				BaseAgent* agent = loadedUnits.at(i);
				agent->LockAgent();
				if (isAlive() && agent->isAlive()) {
					checkStimTrick(agent);
				}
				agent->UnLockAgent();
			}
		}
	}

	if (!unit->isBeingConstructed()) {
		if ((int)loadedUnits.size() < 4) {
			vector<Squad*> squads = Commander::getInstance()->getSquads();
			for (int i = 0; i < (int)squads.size(); i++) {
				//Look up the squads that are assigned to bunker defence
				if (squads.at(i)->isBunkerDefend()) {
					vector<BaseAgent*> agents = squads.at(i)->getMembers();
					//and load each member into a bunker (if not already in one)
					for (int a = 0; a < (int)agents.size(); a++) {
						BaseAgent* agent = agents.at(a);
						if (agent->isAlive() && agent->isOfType(UnitTypes::Terran_Marine)) {
							MarineAgent* marine = (MarineAgent*)agent;
							if (!marine->isBunkered()) {
								//Broodwar->printf("Assign Marine %d to bunker", marine->getUnitID());
								marine->assignToBunkerDefend(unit);
								loadedUnits.push_back(marine);
							}
						}
						if (agent->isAlive() && agent->isOfType(UnitTypes::Terran_Firebat)) {
							FirebatAgent* firebat = (FirebatAgent*)agent;
							if (!firebat->isBunkered()) {
								//Broodwar->printf("Assign Marine %d to bunker", marine->getUnitID());
								firebat->assignToBunkerDefend(unit);
								loadedUnits.push_back(firebat);
							}
						}
					}
				}
			}
		}
	}
}

void BunkerAgent::removeFromSquads(BaseAgent* agent) {
	if(agent == NULL)
		return;

	int squadID = agent->getSquadID();
	if (squadID != -1) {
		Squad* squad = Commander::getInstance()->getSquad(squadID);
		if (squad != NULL) {
			squad->removeMember(agent);
		}
	}
}

void BunkerAgent::checkStimTrick(BaseAgent* agent) {
	UnitAgent* cAgent = (UnitAgent*)agent;
	UnitType type = cAgent->getUnit()->getType();
	if (cAgent->enemyAttackingUnitsWithinRange(6 * 32, unit->getTilePosition()) > 0) {
		//We are being attacked.
		Unit* soldier = cAgent->getUnit();

		if (soldier->isLoaded() && !soldier->isStimmed()) {
			//1. Unload all units
			unit->unloadAll();
			return;
		}
		if (!soldier->isLoaded() && !soldier->isStimmed()) {
			//2. Use Stim Packs
			//Broodwar->printf("Bunkered agent %s using Stim Packs", type.getName().c_str());
			soldier->useTech(TechTypes::Stim_Packs);
			return;
		}
		if (!soldier->isLoaded() && soldier->isStimmed()) {
			//Broodwar->printf("Bunkered agent %s re-entering Bunker", type.getName().c_str());
			//3. Enter Bunker again
			soldier->rightClick(unit);
			return;
		}
	}
}

string BunkerAgent::getTypeName() {
	return "BunkerAgent";
}

bool BunkerAgent::shallEnterBunker(BaseAgent* agent) {
	//Check if the Bunker is finished or not
	if (unit->isBeingConstructed()) {
		return false;
	}

	//Check if the unit is already planned to be in the Bunker.
	for (int i = 0; i < (int)loadedUnits.size(); i++) {
		if (loadedUnits.at(i)->getUnitID() == agent->getUnitID()) {
			return true;
		}
	}

	//Check if the Bunker is full
	if (noMarines + noFirebats >= 4) {
		return false;
	}

	//Or check if we have the right setup of units in the Bunker
	int prefMarines = 4;
	int prefFirebats = 0;
	if (ExplorationManager::enemyIsProtoss() || ExplorationManager::enemyIsUnknown() || ExplorationManager::enemyIsZerg()) {
		prefMarines = 2;
		prefFirebats = 2;
	}

	if (agent->isOfType(UnitTypes::Terran_Marine) && noMarines < prefMarines) {
		return true;
	}
	if (agent->isOfType(UnitTypes::Terran_Firebat) && noFirebats < prefFirebats) {
		return true;
	}
	return false;
}

/*void BunkerAgent::loadUnit(BaseAgent* agent) {
	agent->getUnit()->rightClick(unit);

	bool add = true;
	for (int i = 0; i < (int)loadedUnits.size(); i++) {
		if (loadedUnits.at(i)->getUnitID() == agent->getUnitID()) {
			add = false;
		}
	}
	if (add) {
		loadedUnits.push_back(agent);
		agent->getAgentReference(AgentRemovedEvent, this);
		if (agent->isOfType(UnitTypes::Terran_Marine)) noMarines++;
		if (agent->isOfType(UnitTypes::Terran_Firebat)) noFirebats++;
	}
}*/

void BunkerAgent::printInfo() {
	Broodwar->printf("[%d] (%s) with %d loaded agents", unitID, getTypeName().c_str(), (int)loadedUnits.size());
}

int BunkerAgent::AgentRemovedEvent(void* BaseAgentRemoved, void* ObjectToStore)
{
	Broodwar->printf("BunkerAgent::AgentRemovedEvent");
	BunkerAgent* bunkerAgent = (BunkerAgent*)ObjectToStore;
	for(int i=0;i!=(int)bunkerAgent->loadedUnits.size();i++)
	{
		if(bunkerAgent->loadedUnits.at(i)->matches(((BaseAgent*)BaseAgentRemoved)->getUnit()))
		{
			bunkerAgent->loadedUnits.erase(bunkerAgent->loadedUnits.begin() + i);
			return 1;
		}
	}
	return 0;
}
