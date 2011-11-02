#ifndef __EVOLUTIONCHAMBERAGENT_H__
#define __EVOLUTIONCHAMBERAGENT_H__

#include "StructureAgent.h"

using namespace BWAPI;
using namespace std;

/** The EvolutionChamberAgent handles Terran EvolutionChamber buildings.
 */
class EvolutionChamberAgent : public StructureAgent {

private:

public:
	EvolutionChamberAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
};

#endif