#ifndef __COMSATAGENT_H__
#define __COMSATAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The ComsatAgent handles Terran Comsat Station addon.
 *
 * Implemented abilities:
 * - 
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class ComsatAgent : public StructureAgent {

private:
	int lastSweepFrame;

public:
	ComsatAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();

	/** Uses scanner sweep to reveal an area. */
	bool sweepArea(TilePosition target);
	
};

#endif