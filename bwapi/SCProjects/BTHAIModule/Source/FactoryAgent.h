#ifndef __FACTORYAGENT_H__
#define __FACTORYAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The FactoryAgent handles Terran Factory buildings.
 *
 * Implemented abilities:
 * - Can build the Machine Shop addon.
 * - Trains Vultures.
 * - If Machine Shop addon has been created, trains Siege Tanks.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class FactoryAgent : public StructureAgent {

private:
	int hasMachineShop;

public:
	FactoryAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();

	/** Used when assigning the Factory to build the Machine Shop addon. */
	void assignToBuild(UnitType type);

	/** Used to print info about this agent to the screen. */
	void printInfo();
};

#endif