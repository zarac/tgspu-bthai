#ifndef __CYBERNETICSCOREAGENT_H__
#define __CYBERNETICSCOREAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The CyberneticsCoreAgent handles Protoss Cybernetics Core buildings.
 *
 * Implemented abilities:
 * - Upgrades to Air Armor and Air Weapons Level 1.
 * - Upgrades to Singularity Charge.
 *
 * TODO:
 * - Add other Levels.
 * - Test when to upgrade Air Armor and Weapons.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class CyberneticsCoreAgent : public StructureAgent {

private:

public:
	CyberneticsCoreAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
	
};

#endif