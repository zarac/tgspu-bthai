#ifndef __BUILDPLANNER_H__
#define __BUILDPLANNER_H__

#include <BWAPI.h>

using namespace BWAPI;
using namespace std;

/** The BuildPlanner class contains the build order for all buildings and addons that will be constructed during the course
 * of a game. This class is not directly used, instead each race (Terran, Protoss, Zerg) has their own build order class
 * that extends BuildPlanner.
 *
 * The BuildPlanner is implemented as a singleton class. Each class that needs to access BuildPlanner can request an instance,
 * and all classes shares the same BuildPlanner instance.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class BuildPlanner {

private:
	static BuildPlanner* instance;

protected:
	BuildPlanner();
	vector<UnitType> buildOrder;
	bool executeOrder(UnitType type);
	bool shallBuildSupplyDepot();
	bool locked;
	int lockedFrame;
	int chokepointNo;

	//Only used by Protoss
	int pylonAtChokePointNo;
	int shieldBatteryAtChokePointNo;

public:
	~BuildPlanner();

	/** Returns the instance to the BuildPlanner that is currently used. */
	static BuildPlanner* getInstance();

	/** Initial setup of the build order. */
	virtual void init() {
	}

	/** Called each update to issue orders. */
	virtual void computeActions() {
	}

	/** Notifies that an own unit has been destroyed. */
	virtual void buildingDestroyed(Unit* building) {
	}

	/** Checks if the minerals around a base is running low. If so, order a new base. */
	bool mineralsRunningLow();

	/** When a request to construct a new building is issued, no construction are
	 * allowed until the worker has moved to the buildspot and started constructing
	 * the building. This is to avoid that the needed resources are not used up by
	 * other build orders. During this time the BuildPlanner is locked, and new 
	 * construction can only be done when unlock has been called. */
	virtual void unlock(UnitType type);

	/** Returns true if the BuildPlanner is locked. */
	bool isLocked();

	/** Returns the next chokepoint around the own base. If for example
	 * there are two chokepoint, the method switches between 0 and 1. */
	int getChokepointNo();

	/** Returns the number of frames to wait for a locked timeout for a certain
	 * type of building. */
	static int getLockedTimer(UnitType type);

	/** Returns true if the player is Terran. */
	static bool isTerran();

	/** Returns true if the player is Protoss. */
	static bool isProtoss();

	/** Returns true if the player is Zerg. */
	static bool isZerg();

	/** Used to print some info to the screen. */
	virtual void printInfo() {
	}
};

#endif