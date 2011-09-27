#ifndef __SHUTTLEAGENT_H__
#define __SHUTTLEAGENT_H__

#include <BWAPI.h>
#include "UnitAgent.h"
using namespace BWAPI;
using namespace std;

/** The ShuttleAgent handles Protoss Shuttle flying transport units.
 *
 * Implemented special abilities:
 * - 
 *
 * TODO:
 * - Missing general logic for transports.
 * 
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class ShuttleAgent : public UnitAgent {

private:

public:
	ShuttleAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for unit agents. */
	string getTypeName();
};

#endif