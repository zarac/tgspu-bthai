#ifndef __PROTOSSCOMMANDER_H__
#define __PROTOSSCOMMANDER_H__

#include "Commander.h"

using namespace BWAPI;
using namespace std;

/** This is the Protoss implementation of the Commander class. See
 * Commander for more info.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class ProtossCommander : public Commander {

private:
	void addMainAttackSquad();

public:
	ProtossCommander();
	~ProtossCommander();

	/** Called each update to issue orders. */
	void computeActions();

};

#endif