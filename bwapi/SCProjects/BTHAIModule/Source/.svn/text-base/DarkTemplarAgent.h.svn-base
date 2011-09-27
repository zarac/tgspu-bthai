#ifndef __DARKTEMPLARAGENT_H__
#define __DARKTEMPLARAGENT_H__

#include <BWAPI.h>
#include "UnitAgent.h"
using namespace BWAPI;
using namespace std;

/** The DarkTemplarAgent handles Protoss Dark Templar units.
 *
 * Implemented special abilities:
 * - 
 *
 * TODO:
 * - Avoid detectors
 * - Merge to Dark Archon
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class DarkTemplarAgent : public UnitAgent {

private:
	Unit* findTarget();

public:
	DarkTemplarAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for unit agents. */
	string getTypeName();
};

#endif