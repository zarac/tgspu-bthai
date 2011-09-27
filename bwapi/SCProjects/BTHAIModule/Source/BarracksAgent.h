#ifndef __BARRACKSAGENT_H__
#define __BARRACKSAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The BarracksAgent handles Terran Barracks buildings.
 *
 * Implemented abilities:
 * - Can train Marines.
 * - If Academy has been built, can train Medics and Firebats.
 *
 * TODO:
 * - Currently Ghosts are not handled by the bot.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class BarracksAgent : public StructureAgent {

private:
	
public:
	BarracksAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
};

#endif