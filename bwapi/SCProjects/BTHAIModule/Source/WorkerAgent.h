#ifndef __WORKERAGENT_H__
#define __WORKERAGENT_H__

#include "BaseAgent.h"
using namespace BWAPI;
using namespace std;

/** The WorkerAgent class handles all tasks that a worker, for example a Terran SCV, can perform. The tasks
 * involves gathering minerals and gas, move to a selected buildspot and construct the specified building,
 * and if Terran SCV, repair a building or tank.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class WorkerAgent : public BaseAgent {

private:
	int currentState;

	UnitType toBuild;
	TilePosition buildSpot;
	bool buildSpotExplored();
	bool areaFree();
	TilePosition findStartPoint();
	int startBuildFrame;

	int lastFrame;

public:
	static const int GATHER_MINERALS = 0;
	static const int GATHER_GAS = 1;
	static const int FIND_BUILDSPOT = 2;
	static const int MOVE_TO_SPOT = 3;
	static const int CONSTRUCT = 4;
	static const int REPAIRING = 5;

	WorkerAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for worker agents. */
	string getTypeName();

	/** Set the state of the worker. I.e. what does it do right now. 
	 * Should only be set if the worker is getting a task not through the functions in this class. Then it is automatic. */
	void setState(int state);

	/** Returns true if the Worker agent can create units of the specified type. */
	bool canBuild(UnitType type);

	/** Assigns the agent to repair a building or tank. */
	bool assignToRepair(Unit* building);

	/** Assigns the unit to construct a building of the specified type. */
	void assignToBuild(UnitType type);

	/** Assigns the agent to continue building a non-finished building. */
	bool assignToFinishBuild(Unit* building);

	/** Returns the state of the agent as text. Good for printouts. */
	string getStateAsText();

	/** Resets a worker to gathering minerals. */
	void reset();

	/** Returns true if this worker is in any of the build states, and is constructing
	 * the specified building. */
	bool isConstructing(UnitType type);
};

#endif