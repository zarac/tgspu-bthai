#ifndef __LAIRAGENT_H__
#define __LAIRAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The LairAgent handles Zerg Lair buildings.
 *
 */
class LairAgent : public StructureAgent {

private:
public:
	LairAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
};

#endif