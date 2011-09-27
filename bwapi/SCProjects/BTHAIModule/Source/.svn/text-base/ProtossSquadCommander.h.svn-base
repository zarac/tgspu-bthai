#ifndef __PROTOSSSQUADCOMMANDER_H__
#define __PROTOSSSQUADCOMMANDER_H__

#include "BaseAgent.h"
#include "SquadCommander.h"

using namespace BWAPI;
using namespace std;

/** This is the Protoss instance of the SquadCommander manager.
 * See SquadCommander for more info.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class ProtossSquadCommander : public SquadCommander {

private:

public:
	ProtossSquadCommander();
	~ProtossSquadCommander();

	/** Called each update to issue orders. */
	void computeActions();

	/** Checks if there are any important units such as buildings or siege tanks that are under attack. If so,
	 * the attack force is ordered to assist the building/unit. Returns true if the attack force has been
	 * used to assist a unit. */
	bool checkImportantUnitsUnderAttack();

	/** Returns true if the unit is important to assist, false if not. All buildings and large expensive units
	 * such as Scouts are considered important, while small units such as zealots are not considered important. */
	bool isImportantUnit(Unit* unit);

	/** Returns the tileposition of the closest enemy building, if any enemy building has been sighted. */
	TilePosition getClosestEnemyBuilding();

	/** Used to print some info to the screen. */
	void printInfo();

};

#endif