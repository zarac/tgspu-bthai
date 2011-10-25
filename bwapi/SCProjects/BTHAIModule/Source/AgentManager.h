#ifndef __AGENTMANAGER_H__
#define __AGENTMANAGER_H__

#include <BWAPI.h>
#include "BaseAgent.h"
#include "TerranBuildPlanner.h"
#include "ProtossBuildPlanner.h"

using namespace BWAPI;
using namespace std;

/** The AgentManager class is a container that holds a list of all active agents in the game. Each unit, worker, building or
 * or addon is assigned to an agent. See the MainAgents, StructureAgents and UnitAgents folders for detailed information
 * about each specific type of agent.
 *
 * The AgentManager is implemented as a singleton class. Each class that needs to access AgentManager can request an instance,
 * and all classes shares the same AgentManager instance.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class AgentManager {

private:
	vector<BaseAgent*> agents;
	AgentManager();
	static AgentManager* instance;
	static bool instanceFlag;

public:
	~AgentManager();

	/** Returns the instance to the AgentManager. */
	static AgentManager* getInstance();

	/** Adds an agent to the container. Is called each time a new
	 * unit is built. */
	void addAgent(Unit* unit);

	/** Takes care of changing agent on morph. (for zerg) */
	void addMorphAgent(Unit* unit);

	/** Removes an agent from the container. Is called each time
	 * a unit is destroyed. The agents are not directly removed, but
	 * set to inactive and are removed during the cleanup. */
	void removeAgent(Unit* unit);

	/** Called each update to issue commands from all active agents. */
	void computeActions();

	/** Returns the current number of active worker units. */
	int getNoWorkers();

	/** Return the worker at the specified location. No other kind of agent is effected. The agent is lock to this thread and has to be unlocked by calling release. */
	BaseAgent* getWorkerAt(int i);

	/** Returns a reference to the first free worker. Free workers are workers that are idle
	 * or gathering minerals. The agent is lock to this thread and has to be unlocked by calling release. */
	BaseAgent* getFreeWorker();

	/** Returns the closest free worker from the specified position, or NULL if not found. */
	BaseAgent* findClosestFreeWorker(TilePosition pos);

	/** Checks if any agent has the task to repair this specified agent. */
	bool isAnyAgentRepairingThisAgent(BaseAgent* repairedAgent);

	/** Returns the number of own units of a specific type. */
	int countNoUnits(UnitType type);

	/** Returns the number of bases the player has. */
	int countNoBases();

	/** Returns the number of own units that is in the attack state. */
	int countNoAttackingUnits();

	/** Returns the number of own units that is in the defend state. */
	int countNoDefendingUnits();

	/** Returns the list of all agents in the container. */
	//vector<BaseAgent*> getAgents();

	/** Return the agent at the specified location. The agent is lock to this thread and has to be unlocked by calling release. */
	BaseAgent* at(int i);

	/** Returns all agents. */
	vector<BaseAgent*> getAgents();

	/** Unlocked the semaphore and gives another thread the possibility to access the object. */
	static void release(BaseAgent* agent);

	/** Returns the number of agents the exists in the vector. */
	int size();

	/** Returns a reference to the agent associated with a specific unit,
	 * or NULL if the unit doesn't exist. */
	BaseAgent* getAgent(int unitID);

	/** Returns the first agent in the list of the specified type, or NULL if not found. */
	BaseAgent* getAgent(UnitType type);
	/** Returns the first agent in the list of the specified type that are in ATTACK or
	 * DEFEND state, or NULL if not found. */
	BaseAgent* getCombatAgent(UnitType type);

	/** Returns the closest agent in the list of the specified type, or NULL if not found. */
	BaseAgent* getClosestAgent(TilePosition pos, UnitType type);

	/** Returns the closest base agent (Terran Command Center, Protoss Nexus), in the list,
	 * or NULL if not found. */
	BaseAgent* getClosestBase(TilePosition pos);

	/** Removes inactive agents from the container. Shouldn't be called too often. */
	void cleanup();
};

#endif