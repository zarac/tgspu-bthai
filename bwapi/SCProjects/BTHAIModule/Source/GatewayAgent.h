#ifndef __GATEWAYAGENT_H__
#define __GATEWAYAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The GatewayAgent handles Protoss Gateway buildings.
 *
 * Implemented abilities:
 * - Can train Zealots.
 * - If Cybernetics Core has been built, can train Dragoons.
 * - If Templar Archives has been built, can train Dark Templars and High Templars.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class GatewayAgent : public StructureAgent {

private:
	
public:
	GatewayAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
};

#endif