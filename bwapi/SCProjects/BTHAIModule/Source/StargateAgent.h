#ifndef __STARGATEAGENT_H__
#define __STARGATEAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The StargateAgent handles Protoss Stargate buildings.
 *
 * Implemented abilities:
 * - Can train Scouts.
 * - Can train Corsairs.
 * - Can train Arbiters.
 *
 * TODO:
 * - Transports using Carriers has not been implemented.
 * 
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class StargateAgent : public StructureAgent {

private:
	
public:
	StargateAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
};

#endif