#ifndef __TEMPLARARCHIVESAGENT_H__
#define __TEMPLARARCHIVESAGENT_H__

#include <BWAPI.h>
#include "StructureAgent.h"
using namespace BWAPI;
using namespace std;

/** The TemplarArchivesAgent handles Protoss Templar Archive buildings.
 *
 * Implemented abilities:
 * - Research Maelstrom
 * - Researcg Psionic_Storm
 * - Research Mind_Control
 * - Research Hallucination
 * - Upgrades Khaydarin_Amulet
 * - Upgrades Argus_Talisman
 *
 * Note: Upgrades and research are only performed after at least one High Templar
 * unit has been created.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class TemplarArchivesAgent : public StructureAgent {

private:

public:
	TemplarArchivesAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
	
};

#endif