#ifndef __PROTOSSBUILDPLANNER_H__
#define __PROTOSSBUILDPLANNER_H__

#include "BuildPlanner.h"
#include "ProtossCommander.h"

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
class ProtossBuildPlanner : public BuildPlanner {

private:
	bool shallBuildPylon();
	int level;
	ProtossCommander* getCommanderInstance();

public:
	ProtossBuildPlanner();
	~ProtossBuildPlanner();

	/** Initial setup of the build order. */
	void init();

	/** Called each update to issue orders. */
	void computeActions();

	/** Notifies that an own building has been destroyed. */
	void buildingDestroyed(Unit* building);

	/** See BuildPlanner for info. */
	void unlock(UnitType type);

	/** Used by Protoss to keep track of building one Pylon at each chokepoint, to
	 * support Photon Cannons. */
	int getPylonAtChokePointNo();

	/** Used by Protoss to keep track of building one Pylon at each chokepoint, to
	 * support Photon Cannons. */
	void increasePylonAtChokePointNo();

	/** Used by Protoss to keep track of building one Shield Battery at each chokepoint, to
	 * regenerate shields on defending units and buildings. */
	int getShieldBatteryAtChokePointNo();

	/** Used by Protoss to keep track of building one Shield Battery at each chokepoint, to
	 * regenerate shields on defending units and buildings. */
	void increaseShieldBatteryAtChokePointNo();

	/** Used to print some info to the screen. */
	void printInfo();
};

#endif