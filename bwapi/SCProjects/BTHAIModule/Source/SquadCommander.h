#ifndef __SQUADCOMMANDER_H__
#define __SQUADCOMMANDER_H__

#include "BaseAgent.h"

using namespace BWAPI;
using namespace std;

/** The SquadCommander class is the heart of deciding when and where to engage the enemy. It is responsible for deciding when
 * to attack the enemy, where to attack him, and when a retreat is the best option to do.
 *
 * The SquadCommander is implemented as a singleton class. Each class that needs to access SquadCommander can request an instance,
 * and all classes shares the same SquadCommander instance.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class SquadCommander {

private:
	
	static SquadCommander* instance;

protected:
	SquadCommander();

	bool hasBeenAttacked;
	bool isUnit(Unit* unit);

	TilePosition currentGoal;

	int currentState;
	static const int STATE_DEFEND = 0;
	static const int STATE_ATTACK = 1;

public:
	~SquadCommander();

	/** Returns the instance of the class. */
	static SquadCommander* getInstance();

	/** Called each update to issue orders. */
	virtual void computeActions() {
	}

	/** Checks if there are any important units such as buildings or siege tanks that are under attack. If so,
	 * the attack force is ordered to assist the building/unit. Returns true if the attack force has been
	 * used to assist a unit. */
	virtual bool checkImportantUnitsUnderAttack() {
		return false;
	}

	/** Counts the number of enemy units withing range from the start position. */
	int enemyUnitsWithinRange(TilePosition start, int range);

	/** Checks if workers needs to attack. Happens if base is under attack and no offensive units
	 * are available. */
	bool checkWorkersAttack(BaseAgent* base);

	/** Returns true if the unit is important to assist, false if not. All buildings and large expensive units
	 * such as siege tanks and battlecruisers are considered important, while small units such as marines and
	 * vultures are not considered important. */
	virtual bool isImportantUnit(Unit* unit) {
		return false;
	}

	/** Returns the tileposition of the closest enemy building, if any enemy building has been sighted. */
	virtual TilePosition getClosestEnemyBuilding() {
		return TilePosition(-1, -1);
	}

	/** Returns the tileposition of the enemy building closest to the start point,
	 * if any enemy building has been sighted. */
	TilePosition getClosestEnemyBuildingTo(TilePosition start);

	/** Checks if there are any removable obstacles nearby, i.e. minerals with less than 20 resources
	 * left. */
	void checkRemovableObstacles();

	/** Counts the strength of the own attack force. Is using the values an opponent is awarded when
	 * destroying a unit of a specific type. */
	int countOwnStrength();

	/** Counts the strength of the visible enemy force. If perfect information is available, all enemy
	 * units are counted. */
	int estimateEnemyStrength();

	/** Used to print the own and enemy strength to the screen. */
	void printStrength();

	/** Used to print some info to the screen. */
	virtual void printInfo() {
	}

};

#endif