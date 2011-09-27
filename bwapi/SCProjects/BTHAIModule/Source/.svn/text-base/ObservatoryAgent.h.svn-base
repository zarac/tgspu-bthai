#ifndef __OBSERVATORYAGENT_H__
#define __OBSERVATORYAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The ObservatoryAgent handles Protoss Observatory buildings.
 *
 * Implemented abilities:
 * - Upgrades to Sensor Array.
 * - Upgrades to Gravitic Boosters
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class ObservatoryAgent : public StructureAgent {

private:

public:
	ObservatoryAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
	
};

#endif