#ifndef __CONTROLTOWERAGENT_H__
#define __CONTROLTOWERAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The ControlTowerAgent handles Terran Control Tower Starport addons.
 *
 * Implemented abilities:
 * - Researches Wraith Cloaking Field.
 * 
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class ControlTowerAgent : public StructureAgent {

private:

public:
	ControlTowerAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
	
};

#endif