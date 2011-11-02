#ifndef __ZERGBUILDPLANNER_H__
#define __ZERGBUILDPLANNER_H__

#include "BuildPlanner.h"
#include "ZergCommander.h"

using namespace BWAPI;
using namespace std;

/** This is the Protoss implementation of the BuildPlanner class. See BuildPlanner for more details.
 *
 * Protoss bot tactics:
 * - To come.
 *
 * Weaknesses:
 * - To come.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class ZergBuildPlanner : public BuildPlanner {

private:
	bool buildOverlordIfNeeded();
	bool buildHatcheryIfNeeded();
	int level;
	ZergCommander* getCommanderInstance();

public:
	ZergBuildPlanner();
	~ZergBuildPlanner();

	/** Initial setup of the build order. */
	void init();

	/** Called each update to issue orders. */
	void computeActions();

	/** Notifies that an own building has been destroyed. */
	void buildingDestroyed(Unit* building);

	/** See BuildPlanner for info. */
	void unlock(UnitType type);

	/** Used to print some info to the screen. */
	void printInfo();
};

#endif