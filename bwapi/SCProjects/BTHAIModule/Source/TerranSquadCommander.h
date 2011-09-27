#ifndef __TERRANSQUADCOMMANDER_H__
#define __TERRANSQUADCOMMANDER_H__

#include "BaseAgent.h"
#include "SquadCommander.h"

using namespace BWAPI;
using namespace std;

/** This is the Terran instance of the SquadCommander manager.
 * See SquadCommander for more info.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class TerranSquadCommander : public SquadCommander {

private:
	int lastSweepFrame;

public:
	TerranSquadCommander();
	~TerranSquadCommander();

	/** Called each update to issue orders. */
	void computeActions();

	/** Checks if there are any important units such as buildings or siege tanks that are under attack. If so,
	 * the attack force is ordered to assist the building/unit. Returns true if the attack force has been
	 * used to assist a unit. */
	bool checkImportantUnitsUnderAttack();

	/** Checks if there are any unfinished buildings that does not have an SCV working on them. Terran only. */
	bool checkUnfinishedBuildings();
	
	/** Returns true if the unit is important to assist, false if not. All buildings and large expensive units
	 * such as siege tanks and battlecruisers are considered important, while small units such as marines and
	 * vultures are not considered important. */
	bool isImportantUnit(Unit* unit);

	/** Assigns a worker to repair the specified agent. */
	void repair(BaseAgent* agent);

	/** Assigns a worker to finish constructing an interrupted building. */
	void finishBuild(BaseAgent* agent);

	/** Returns the tileposition of the closest enemy building, if any enemy building has been sighted. */
	TilePosition getClosestEnemyBuilding();

	/** Used to print some info to the screen. */
	void printInfo();

};

#endif