#ifndef __NEXUSAGENT_H__
#define __NEXUSAGENT_H__

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
class NexusAgent : public StructureAgent {

private:
	int idealNoWorkers;
	int level;

	bool hasSentWorkers;

public:
	NexusAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
};

#endif