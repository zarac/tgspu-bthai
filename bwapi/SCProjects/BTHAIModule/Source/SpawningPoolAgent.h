#ifndef __SPAWNINGPOOLAGENT_H__
#define __SPAWNINGPOOLAGENT_H__

#include "StructureAgent.h"

using namespace BWAPI;
using namespace std;

/** The SpawningPoolAgent handles Terran SpawningPoolAgent buildings.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class SpawningPoolAgent : public StructureAgent {

private:

public:
	SpawningPoolAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
};

#endif