#ifndef __MARINEAGENT_H__
#define __MARINEAGENT_H__

#include <BWAPI.h>
#include "UnitAgent.h"
#include "BunkerAgent.h"
using namespace BWAPI;
using namespace std;

/** The MarineAgent handles Terran Marine units.
 *
 * Implemented special abilities:
 * - The marine uses stim pack (if researched) when there are enemy units within firerange
 * and the health of the marine is not too low for the unit to be stimmed.
 * - If there are any free spots in any bunker, the marine enters the bunker.
 * - Marines can be used as explorers.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class MarineAgent : public UnitAgent {

private:
	Unit* myBunker;

public:
	MarineAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for unit agents. */
	string getTypeName();

	/** Returns true if this unit is assigned to a Bunker. */
	bool isBunkered();

	/** Assigns this unit to defend from a Bunker. */
	void assignToBunkerDefend(Unit* bunker);
};

#endif