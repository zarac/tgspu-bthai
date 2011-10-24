#ifndef __ZERGLINGAGENT_H__
#define __ZERGLINGAGENT_H__

#include <BWAPI.h>
#include "UnitAgent.h"
using namespace BWAPI;
using namespace std;

/** The ZerglingAgent handles Zerg Zergling units.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class ZerglingAgent : public UnitAgent {

private:
	Unit* findTarget();

public:
	ZerglingAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for unit agents. */
	string getTypeName();
};

#endif