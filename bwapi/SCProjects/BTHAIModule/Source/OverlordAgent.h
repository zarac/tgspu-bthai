#ifndef __OVERLORDAGENT_H__
#define __OVERLORDAGENT_H__

#include <BWAPI.h>
#include "UnitAgent.h"
using namespace BWAPI;
using namespace std;

/** The OverlordAgent handles Zerg Overlord units.
 *
 */
class OverlordAgent : public UnitAgent {

private:
	Unit* findTarget();

public:

	OverlordAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for unit agents. */
	string getTypeName();
};

#endif