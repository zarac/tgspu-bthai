#ifndef __WRAITHAGENT_H__
#define __WRAITHAGENT_H__

#include <BWAPI.h>
#include "UnitAgent.h"
using namespace BWAPI;
using namespace std;

/** The WraithAgent handles Terran Wraith flying units.
 *
 * Implemented special abilities:
 * - If there are enemy units within firerange, the Wraith uses Cloaking 
 * Field (if researched).
 * - Wraiths can be used as explorers.
 *
 * TODO:
 * - Cloaking Field shall only be used if there are detectors around.
 * - If there is a detector around, but no enemy units that can attack flying units, there
 * is no need to use Cloaking Field.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class WraithAgent : public UnitAgent {

private:
	bool checkUseCloak();
public:
	WraithAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for unit agents. */
	string getTypeName();
};

#endif