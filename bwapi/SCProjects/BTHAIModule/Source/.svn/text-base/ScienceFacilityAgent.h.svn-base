#ifndef __SCIENCEFACILITYAGENT_H__
#define __SCIENCEFACILITYAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The ScienceFacilityAgent handles Terran Science Facility buildings.
 *
 * Implemented abilities:
 * - Builds the Covert Ops addon to support Ghosts.
 * - Can build the Physics Lab addon to support Battlecruisers.
 * - Research the EMP Shockwave and Irradiate abilities for Science Vessels.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class ScienceFacilityAgent : public StructureAgent {

private:
	int hasAddon;
	UnitType toBuild;

public:
	ScienceFacilityAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();

	/** Used when building an addon. */
	void assignToBuild(UnitType type);
};

#endif