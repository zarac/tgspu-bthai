#ifndef __TERRANCOMMANDER_H__
#define __TERRANCOMMANDER_H__

#include "Commander.h"

using namespace BWAPI;
using namespace std;

/** This is the Terran implementation of the Commander class. See
 * Commander for more info.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class TerranCommander : public Commander {

private:
	
public:
	TerranCommander();
	~TerranCommander();

	/** Called each update to issue orders. */
	void computeActions();

	/** Checks if there are any unfinished buildings that does not have an SCV working on them. Terran only. */
	bool checkUnfinishedBuildings();

	/** Check if there are any important buildings or units to repair. */
	bool checkRepairUnits();

	/** Returns true if the unit is important to assist, false if not. All buildings and large expensive units
	 * such as siege tanks and battlecruisers are considered important, while small units such as marines and
	 * vultures are not considered important. */
	bool isImportantUnit(Unit* unit);

	/** Assigns a worker to repair the specified agent. */
	void repair(BaseAgent* agent);

	/** Assigns a worker to finish constructing an interrupted building. */
	void finishBuild(BaseAgent* agent);

	void addMainAttackSquad();
	void addBunkerSquad();

};

#endif