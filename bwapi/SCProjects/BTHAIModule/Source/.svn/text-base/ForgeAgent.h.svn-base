#ifndef __FORGEAGENT_H__
#define __FORGEAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The ForgeAgent handles Protoss Forge buildings.
 *
 * Implemented abilities:
 * - Upgrades to Plasma Shields.
 * - Upgrades to Ground Armor.
 * - Upgrades to Ground Weapons.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class ForgeAgent : public StructureAgent {

private:
	int level;

public:
	ForgeAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
	
};

#endif