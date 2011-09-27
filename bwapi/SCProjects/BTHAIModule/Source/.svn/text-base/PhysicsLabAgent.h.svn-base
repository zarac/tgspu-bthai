#ifndef __PHYSICSLABAGENT_H__
#define __PHYSICSLABAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The PhysicsLabAgent handles Terran Physics Lab Science Facility addon.
 *
 * Implemented abilities:
 * - Researches the Yamato Gun.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class PhysicsLabAgent : public StructureAgent {

private:

public:
	PhysicsLabAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
	
};

#endif