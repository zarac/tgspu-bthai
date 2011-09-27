#ifndef __COMMANDER_H__
#define __COMMANDER_H__

#include "Squad.h"
#include "BaseAgent.h"

using namespace BWAPI;
using namespace std;

/** The Commander is a replacement for the older SquadCommander manager. The class is the heart of
 * deciding when and where to engage the enemy. It is responsible for deciding when
 * to attack the enemy, where to attack him, and when a retreat is the best option to do.
 * All units are grouped into Squads, and the Commander directs the different Squads to attack
 * or defend positions. Each Squad are in turn responsible for handle the task it has been assigned.
 *
 * The Commander is implemented as a singleton class. Each class that needs to access Commander can request an instance,
 * and all classes shares the same Commander instance.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class Commander {

protected:
	vector<Squad*> squads;
	int currentID;
	static Commander* instance;

	int chokepointNo;

	int currentState;
	static const int DEFEND = 0;
	static const int ATTACK = 1;

	Commander();

public:
	~Commander();

	/** Returns the instance of the class. */
	static Commander* getInstance();

	/** Called each update to issue orders. */
	virtual void computeActions() {
	}

	/** Called each time a unit is created. The unit is then
	 * placed in a Squad. */
	void unitCreated(BaseAgent* agent);

	/** Called each time a unit is destroyed. The unit is then
	 * removed from its Squad. */
	void unitDestroyed(BaseAgent* agent);

	/* Checks if the specified unittype needs to be built. */
	bool needUnit(UnitType type);

	/** Returns the Squad with the specified id, or NULL if not found. */
	Squad* getSquad(int id);

	/** Returns all Squads. */
	vector<Squad*> getSquads();

	/** Returns the position of the closest enemy building from the start position,
	 * or TilePosition(-1,-1) if not found. */
	TilePosition getClosestEnemyBuilding(TilePosition start);
	
	/** Returns the number of active offensive squads within maxRange of 
	 * the center position. */
	int noOffensiveSquadsWithin(TilePosition center, int maxRange);

	/** Checks if workers needs to attack. Happens if base is under attack and no offensive units
	 * are available. */
	bool checkWorkersAttack(BaseAgent* base);

	/** Checks if a base is under attack. If it is, find a defend squad to assist it. */
	bool checkBaseUnderAttack();

	/** Tries to find a free squad to assist a building. */
	void assistBuilding(BaseAgent* building);

	/** Counts the number of enemy units withing range from the start position. */
	int enemyUnitsWithinRange(TilePosition start, int range);

	/** Checks if there are any removable obstacles nearby, i.e. minerals with less than 20 resources
	 * left. */
	void checkRemovableObstacles();

	/** Returns the next chokepoint to send defensive squads to. */
	BWTA::Chokepoint* getNextChokePoint();

};

#endif