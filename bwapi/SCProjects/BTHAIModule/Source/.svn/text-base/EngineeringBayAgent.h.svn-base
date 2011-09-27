#ifndef __ENGINEERINGBAYAGENT_H__
#define __ENGINEERINGBAYAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The EngineeringBayAgent handles Terran Engineering Bay buildings.
 *
 * Implemented abilities:
 * - Upgrades to Infantry Weapons and Infantry Armor Level 1.
 *
 * TODO:
 * - Upgrades to Level 2 are missing. 
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class EngineeringBayAgent : public StructureAgent {

private:

public:
	EngineeringBayAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
	
};

#endif