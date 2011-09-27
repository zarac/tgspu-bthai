#ifndef __FIREBATAGENT_H__
#define __FIREBATAGENT_H__

#include <BWAPI.h>
#include "UnitAgent.h"
#include "BunkerAgent.h"
using namespace BWAPI;
using namespace std;

/** The FirebatAgent handles Terran Firebat units.
 *
 * Implemented special abilities:
 * - The firebat uses stim pack (if researched) when there are enemy units within firerange
 * and the health of the firebat is not too low for the unit to be stimmed.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class FirebatAgent : public UnitAgent {

private:
	Unit* myBunker;

public:
	FirebatAgent(Unit* mUnit);

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