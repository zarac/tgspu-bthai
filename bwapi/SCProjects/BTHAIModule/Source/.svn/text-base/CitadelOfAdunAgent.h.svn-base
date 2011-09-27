#ifndef __CITADELOFADUNAGENT_H__
#define __CITADELOFADUNAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The CitadelOfAdunAgent handles Protoss Citadel of Adun buildings.
 *
 * Implemented abilities:
 * - Upgrades Leg Enchancements.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class CitadelOfAdunAgent : public StructureAgent {

private:

public:
	CitadelOfAdunAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
	
};

#endif