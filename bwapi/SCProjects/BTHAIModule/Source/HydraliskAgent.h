#ifndef __HYDRALISKAGENT_H__
#define __HYDRALISKAGENT_H__

#include <BWAPI.h>
#include "UnitAgent.h"
using namespace BWAPI;
using namespace std;

/** The HydraliskAgent handles Zerg Hydralisk units.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class HydraliskAgent : public UnitAgent {

private:
	Unit* findTarget();

public:
	HydraliskAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for unit agents. */
	string getTypeName();
};

#endif