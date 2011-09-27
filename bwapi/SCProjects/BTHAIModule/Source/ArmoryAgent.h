#ifndef __ARMORYAGENT_H__
#define __ARMORYAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The ArmoryAgent handles Terran Armory buildings.
 *
 * Implemented abilities:
 * - Upgrades to Vehicle Weapons and Vehicle Plating Level 1.
 *
 * TODO:
 * - Upgrades to Level 2 are missing. 
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class ArmoryAgent : public StructureAgent {

private:

public:
	ArmoryAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
	
};

#endif