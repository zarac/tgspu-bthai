#ifndef __HIGHTEMPLARAGENT_H__
#define __HIGHTEMPLARAGENT_H__

#include <BWAPI.h>
#include "UnitAgent.h"
using namespace BWAPI;
using namespace std;

/** The HighTemplarAgent handles Protoss High Templar units.
 *
 * Implemented special abilities:
 * - 
 *
 * TODO:
 * - Merge to Archon
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class HighTemplarAgent : public UnitAgent {

private:
	Unit* findPsiStormTarget();
	BaseAgent* findHallucinationTarget();
	BaseAgent* findArchonTarget();
	bool hasCastTransform;

public:
	HighTemplarAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for unit agents. */
	string getTypeName();
};

#endif