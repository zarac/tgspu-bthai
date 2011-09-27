#ifndef __ASSIMILATORAGENT_H__
#define __ASSIMILATORAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The AssimilatorAgent handles Protoss Assimilator buildings.
 *
 * Implemented abilities:
 * - Makes sure each Assimilator has 3 Probes assigned to gather gas.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class AssimilatorAgent : public StructureAgent {

private:
	vector<BaseAgent*> assignedWorkers;

public:
	AssimilatorAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
};

#endif