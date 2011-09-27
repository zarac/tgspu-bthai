#ifndef __ROBOTICSFACILITYAGENT_H__
#define __ROBOTICSFACILITYAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The RoboticsFacilityAgent handles Protoss Robotic Facility buildings.
 *
 * Implemented abilities:
 * - If Robotics Support Bay has been created, trains Reavers.
 * - If Observatory has been created, trains Observers.
 *
 * TODO:
 * - Shuttles (transport unit) aren't used.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class RoboticsFacilityAgent : public StructureAgent {

private:

public:
	RoboticsFacilityAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();

};

#endif