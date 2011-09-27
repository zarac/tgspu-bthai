#ifndef __COVERTOPSAGENT_H__
#define __COVERTOPSAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The CovertOpsAgent handles Terran Covert Ops Science Facility addon.
 *
 * Implemented abilities:
 * - Researches Personnel Cloaking
 * - Researches Lockdown
 * - Upgrades Ocular Implants
 * - Upgrades Moebius Reactor
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class CovertOpsAgent : public StructureAgent {

private:

public:
	CovertOpsAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
	
};

#endif