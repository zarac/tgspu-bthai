#ifndef __SUNKENCOLONYAGENT_H__
#define __SUNKENCOLONYAGENT_H__

#include "StructureAgent.h"

using namespace BWAPI;
using namespace std;

/** The SunkenColonyAgent handles Terran SunkenColony buildings.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class SunkenColonyAgent : public StructureAgent {

private:

public:
	SunkenColonyAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
};

#endif