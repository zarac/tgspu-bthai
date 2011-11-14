#ifndef __BASEAGENT_H__
#define __BASEAGENT_H__

#include <windows.h>
#include <BWAPI.h>
#include <BWTA.h>
#include "BWTAExtern.h"
using namespace BWAPI;
using namespace std;

/** Callback */
typedef int (*AgentRemovedEvent)(
    void* BaseAgentRemoved,
	void* Object
);

/* Structs */
#pragma pack(1)
typedef struct  _AGENTREMOVEDPACK {
	AgentRemovedEvent eventCallback;
	void* ObjectToStore;
} AGENTREMOVEDPACK, *PAGENTREMOVEDPACK;
#pragma pack()

/** The BaseAgent is the base agent class all agent classes directly or indirectly must extend. It contains some
 * common logic that is needed for several other agent implementations.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class BaseAgent {

private:
	vector<AGENTREMOVEDPACK> eventCallbacks;

protected:
	Unit* unit;
	int unitID;
	int squadID;
	bool alive;
	int lastHitPoints;
	string info;
	bool bBlock;

public:
	BaseAgent();
	BaseAgent(Unit* mUnit);
	~BaseAgent();

	/** Object used to lock the thread for thread safe execution. */
	HANDLE hLockEvent;

	/** Has to be called before modifing the object. In other case another thread my change the object. */
	void LockAgent();

	/** Unlocks the agent again. If not called the agent will be locked for other threads. */
	void UnLockAgent();

	/** Returns a pointer to the agent and registers the callback to be called when the agent is removed. */
	BaseAgent* getAgentReference(AgentRemovedEvent eventCallback, void* ObjectToStore);

	/** Removes the callback. Perhaps is the target killed? */
	void removeAgentReference(AgentRemovedEvent eventCallback, void* ObjectToStore);

	/** Called each update to issue orders. */
	virtual void computeActions() {
	}

	/** Returns the unique type name for the agent type. */
	virtual string getTypeName() {
		return "BaseAgent";
	}

	/** Assigns the agent to build the specified type of unit. */
	virtual void assignToBuild(UnitType type) {
	}

	/** Assigns the agent to repair a unit. */
	virtual bool assignToRepair(Unit* building) {
		return false;
	}

	/** Assigns the agent to continue building a non-finished building. */
	virtual bool assignToFinishBuild(Unit* building) {
		return false;
	}

	/** Sets the move goal for this agent. */
	virtual void setGoal(TilePosition goal) {
	}

	/** Removes the current move goal this agent has. */
	virtual void clearGoal() {
	}

	/** Returns the move goal for this agent. */
	virtual TilePosition getGoal() {
		return TilePosition(-1, -1);
	}

	/** Assigns this agent to explore the game world. */
	virtual void assignToExplore() {
	}

	/** Assigns this agent to be in the attack force. */
	virtual void assignToAttack() {
	}

	/** Assigns this agent to defend the base. */
	virtual void assignToDefend() {
	}

	/** Returns true if this agent is used to explore the game world. */
	virtual bool isExploring() {
		return false;
	}

	/** Returns true if this agent is used to attack the enemy. */
	virtual bool isAttacking() {
		return false;
	}

	/** Returns true if this agent is defending the own base. */
	virtual bool isDefending() {
		return false;
	}

	/** Used to print info about this agent to the screen. */
	virtual void printInfo();

	/** Sets additional information about this agent to be shown when using printInfo(). */
	void setInfo(string mInfo);

	/** Returns true if this agent can build units of the specified type. */
	virtual bool canBuild(UnitType type);

	/** Returns the unique id for this agent. Agent id is the same as the id of the unit
	 * assigned to the agent. */
	int getUnitID();

	/** Returns a reference to the unit assigned to this agent. */
	Unit* getUnit();

	/** Called when the unit assigned to this agent is destroyed. */
	void destroyed();

	/** Returns true if this agent is active, i.e. the unit is not destroyed. */
	bool isAlive();

	/** Returns true if the specified unit is the same unit assigned to this agent. */
	bool matches(Unit* mUnit);

	/** Returns true if the agent is of the specified type. */
	bool isOfType(UnitType type);

	/** Returns true if the unit is of the specified type. */
	static bool isOfType(Unit* mUnit, UnitType type);

	/** Returns true if mType is the same UnitType as toCheckType. */
	static bool isOfType(UnitType mType, UnitType toCheckType);

	/** Checks if there are any enemy detector units withing range of the
	 * specified position. True if there is, false if not. */
	bool isDetectorWithinRange(TilePosition pos, int range);

	/** Returns true if this agent is a building. */
	bool isBuilding();

	/** Returns true if this agent is a worker. */
	bool isWorker();

	/** Returns true if this agent is a free worker, i.e. is idle or is gathering minerals. */
	bool isFreeWorker();

	/** Returns true if this agent is a combat unit. */
	bool isUnit();

	/** Returns true if this agent is under attack, i.e. lost hitpoints since last check. */
	bool isUnderAttack();

	/** Returns true if this agent is damaged. */
	bool isDamaged();

	/** Orders the Terran Comsat Station to do a Scanner Sweep at the specified area. */
	static bool doScannerSweep(TilePosition pos);

	/** Returns the chokepoint closest to the own base. */
	static BWTA::Chokepoint* getClosestChokePoint();

	/** Returns the chokepoint closest to the own base, although not within given distance. */
	static BWTA::Chokepoint* getClosestChokePoint(int distance);

	/** Returns the specific chokepoint (0, 1, 2, ...) around the own base. */
	static BWTA::Chokepoint* getChokePoint(int nr);

	/** Returns the number of chokepoints around the own base. */
	static int getNoChokePoints();

	/** Assigns this agent to the squad with the specified id. */
	void setSquadID(int id);

	/** Returns the squad this agent belongs to, or -1 if it doesnt
	 * belong to any squad. */
	int getSquadID();

};

#endif