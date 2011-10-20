#ifndef __HATCHERYAGENT_H__
#define __HATCHERYAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The NexusAgent handles Protoss Nexus buildings.
 *
 * Implemented abilities:
 * - Trains and keeps the number of Probes (workers) up.
 * 
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class HatcheryAgent : public StructureAgent {

private:
	int idealNoWorkers;
	int level;

	bool hasSentWorkers;

public:
	HatcheryAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
};

#endif