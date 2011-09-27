#ifndef __MEDICAGENT_H__
#define __MEDICAGENT_H__

#include <BWAPI.h>
#include "UnitAgent.h"
using namespace BWAPI;
using namespace std;

/** The MedicAgent handles Terran Medics.
 *
 * Implemented special abilities:
 * - Targets and follows own organic units that are in the attacking force.
 * - Heals nearby damaged organic units.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class MedicAgent : public UnitAgent {

private:
	bool checkUnitsToHeal();
	bool checkUnitsToFollow();

public:
	MedicAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for unit agents. */
	string getTypeName();

	/** Returns true if the specified own unit is a good target for the medic to follow and heal. Good targets
	 * must be biological, must be in the attack force, and must not be loaded into a building or transport. */
	static bool isMedicTarget(Unit* mUnit);
};

#endif