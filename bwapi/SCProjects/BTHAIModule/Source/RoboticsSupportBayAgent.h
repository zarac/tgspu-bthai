#ifndef __ROBOTICSSUPPORTBAYAGENT_H__
#define __ROBOTICSSUPPORTBAYAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The RoboticsSupportBayAgent handles Protoss Robotics Support Bay buildings.
 *
 * Implemented abilities:
 * - Upgrades Scarab Damage.
 * - Upgrades Reaver Capacity.
 * - Upgrades Gravitic Drive.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class RoboticsSupportBayAgent : public StructureAgent {

private:

public:
	RoboticsSupportBayAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
	
};

#endif