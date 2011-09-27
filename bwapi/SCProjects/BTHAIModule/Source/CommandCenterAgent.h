#ifndef __COMMANDCENTERAGENT_H__
#define __COMMANDCENTERAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The CommandCenterAgent handles Terran Command Center buildings.
 *
 * Implemented abilities:
 * - Trains and keeps the number of SCVs (workers) up. Is implemented in levels
 * where the preferred number of SCVs are higher at higher levels, i.e. later in
 * the game.
 * 
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class CommandCenterAgent : public StructureAgent {

private:
	int idealNoWorkers;
	int level;
	int hasAddon;
	UnitType toBuild;

	bool hasSentWorkers;

public:
	CommandCenterAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();

	/** Used when building an addon. */
	void assignToBuild(UnitType type);
};

#endif