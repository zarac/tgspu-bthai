#ifndef __STARPORTAGENT_H__
#define __STARPORTAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The StarportAgent handles Terran Starport buildings.
 *
 * Implemented abilities:
 * - Trains Wraiths.
 * - Trains Science Vessels.
 * - If Science Facility with Physics Lab has been built, trains Battlecruisers.
 *
 * TODO:
 * - Valkyries and Dropships are not implemented. 
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class StarportAgent : public StructureAgent {

private:
	int hasAddon;
	UnitType toBuild;

public:
	StarportAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();

	/** Used when building the Control Tower addon. */
	void assignToBuild(UnitType type);
};

#endif