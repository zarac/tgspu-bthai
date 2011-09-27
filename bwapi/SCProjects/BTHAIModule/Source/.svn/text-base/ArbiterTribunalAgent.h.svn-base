#ifndef __ARBITERTRIBUNALAGENT_H__
#define __ARBITERTRIBUNALAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The ArbiterTribunalAgent handles Protoss Arbiter Tribunal buildings.
 *
 * Implemented abilities:
 * - Research Stasis Field
 *
 * TODO:
 * - Add other research/upgrades.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class ArbiterTribunalAgent : public StructureAgent {

private:

public:
	ArbiterTribunalAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
	
};

#endif