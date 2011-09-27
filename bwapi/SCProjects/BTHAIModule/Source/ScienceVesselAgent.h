#ifndef __SCIENCEVESSELAGENT_H__
#define __SCIENCEVESSELAGENT_H__

#include <BWAPI.h>
#include "UnitAgent.h"
using namespace BWAPI;
using namespace std;

/** The ScienceVesselAgent handles Terran Science Vessels units.
 *
 * Implemented special abilities:
 * - If the Science Vessel is under attack, it uses Defense Matrix to shield itself.
 * - If there are organic enemy units within range, Irradiate (if researched) is used.
 * - If there are any shielded enemy units within range, EMP Shockwave (if researched) is
 * used to drain the shield.
 *
 * TODO:
 * - Defense Matrix should be used to shield other important units as well, for example
 * Battlecruisers or Siege Tanks.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class ScienceVesselAgent : public UnitAgent {

private:
	BaseAgent* findImportantUnit();
	bool isImportantUnit(BaseAgent* agent);
	int lastIrradiateFrame;
	
public:
	ScienceVesselAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for unit agents. */
	string getTypeName();
};

#endif