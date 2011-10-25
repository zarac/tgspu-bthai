#ifndef __EXTRACTORAGENT_H__
#define __EXTRACTORAGENT_H__

#include "StructureAgent.h"

using namespace BWAPI;
using namespace std;

/** The ExtractorAgent handles Terran Extractor buildings.
 *
 * Implemented abilities:
 * - Makes sure each Extractor has 3 workers assigned to gather gas.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class ExtractorAgent : public StructureAgent {

private:
	vector<BaseAgent*> assignedWorkers;

public:
	ExtractorAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for structure agents. */
	string getTypeName();
};

#endif