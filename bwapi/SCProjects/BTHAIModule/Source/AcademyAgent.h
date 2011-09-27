#ifndef __ACADEMYAGENT_H__
#define __ACADEMYAGENT_H__

#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The AcademyAgent handles Terran Academy buildings.
 *
 * Implemented abilities:
 * - Upgrades to U 238 Shells and research Stim Packs.
 *
 * TODO:
 * - Research/upgrades for Medics are missing. 
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class AcademyAgent : public StructureAgent {

private:

public:
	AcademyAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
	
};

#endif