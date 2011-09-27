#ifndef __REFINERYAGENT_H__
#define __REFINERYAGENT_H__

#include "StructureAgent.h"

using namespace BWAPI;
using namespace std;

/** The RefineryAgent handles Terran Refinery buildings.
 *
 * Implemented abilities:
 * - Makes sure each Refinery has 3 workers assigned to gather gas.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class RefineryAgent : public StructureAgent {

private:
	vector<BaseAgent*> assignedWorkers;

public:
	RefineryAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
};

#endif