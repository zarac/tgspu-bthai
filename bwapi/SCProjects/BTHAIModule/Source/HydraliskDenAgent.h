#ifndef __HYDRALISKDENAGENT_H__
#define __HYDRALISKDENAGENT_H__

#include "StructureAgent.h"

using namespace BWAPI;
using namespace std;

/** The HydraliskDenAgent handles Terran HydraliskDen buildings.
 */
class HydraliskDenAgent : public StructureAgent {

private:

public:
	HydraliskDenAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
};

#endif