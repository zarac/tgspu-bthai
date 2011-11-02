#ifndef __CREEPCOLONYAGENT_H__
#define __CREEPCOLONYAGENT_H__

#include "StructureAgent.h"

using namespace BWAPI;
using namespace std;

/** The CreepColonyAgent handles Terran CreepColonyAgent buildings.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class CreepColonyAgent : public StructureAgent {

private:

public:
	CreepColonyAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
};

#endif