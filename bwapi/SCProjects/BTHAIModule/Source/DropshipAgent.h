#ifndef __DROPSHIPAGENT_H__
#define __DROPSHIPAGENT_H__

#include <BWAPI.h>
#include "UnitAgent.h"
using namespace BWAPI;
using namespace std;

/** The DropshipAgent handles Terran Dropship flying transport units.
 *
 * Implemented special abilities:
 * - 
 *
 * TODO:
 * - Missing general logic for transport units.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class DropshipAgent : public UnitAgent {

private:

public:
	DropshipAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for unit agents. */
	string getTypeName();
};

#endif