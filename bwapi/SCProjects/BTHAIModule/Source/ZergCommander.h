#ifndef __ZERGCOMMANDER_H__
#define __ZERGCOMMANDER_H__

#include "Commander.h"

using namespace BWAPI;
using namespace std;

/** This is the Zerg implementation of the Commander class. See
 * Commander for more info.
 */
class ZergCommander : public Commander {

private:
	void addMainAttackSquad();
	void addHydraliskSquad();
	void manageLevel();
	int hydraSquads;
	int idealWorkerCount;
	int level;
	Squad* sixPool;

public:
	ZergCommander();
	~ZergCommander();

	/** Called each update to issue orders. */
	void computeActions();
	int getIdealWorkerCount();
	int getLevel();
	int getState();

};

#endif