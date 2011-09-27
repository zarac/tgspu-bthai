#ifndef __SHIELDBATTERYAGENT_H__
#define __SHIELDBATTERYAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The ShieldBatteryAgent handles Protoss Shield Battery buildings.
 *
 * Implemented abilities:
 * - Recharges nearby Shields.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class ShieldBatteryAgent : public StructureAgent {

private:

public:
	ShieldBatteryAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
	
};

#endif