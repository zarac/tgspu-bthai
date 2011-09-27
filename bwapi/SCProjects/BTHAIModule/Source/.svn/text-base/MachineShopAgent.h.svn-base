#ifndef __MACHINESHOPAGENT_H__
#define __MACHINESHOPAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The MachineShopAgent handles Terran Machine Shop Factory addon.
 *
 * Implemented abilities:
 * - Research Siege Mode and Spider Mines.
 * - After that, upgrades to Ion Thrusters.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class MachineShopAgent : public StructureAgent {

private:

public:
	MachineShopAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
	
};

#endif