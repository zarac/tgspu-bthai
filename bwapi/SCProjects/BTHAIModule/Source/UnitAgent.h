#ifndef __UNITAGENT_H__
#define __UNITAGENT_H__

#include <BWAPI.h>
#include "BaseAgent.h"
using namespace BWAPI;
using namespace std;

#define DISABLE_UNIT_AI 0

/** The UnitAgent is the base agent class for all agents handling mobile units. If a unit is created and no
 * specific agent for that type is found, the unit is assigned to a UnitAgents. UnitAgents can attack and
 * assist building under enemy fire, but cannot use any special abilities. To use abilities such as Terran Vultures
 * dropping spider mines, an agent implementation for that unit type must be created.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class UnitAgent : public BaseAgent {

protected:
	int lastFrame;
	TilePosition goal;

	int currentState;
	static const int ATTACK = 1;
	static const int EXPLORE = 2;
	static const int DEFEND = 3;
	static const int BUNKERED = 4;
	
public:
	UnitAgent(Unit* mUnit);
	UnitAgent();

	/** Called each update to issue orders. */
	void computeActions();

	/** Returns the unique type name for unit agents. */
	string getTypeName();

	/** Sets the goal for this unit. Goals are set from either the SquadCommander for attacking
	 * or defending units, or from ExplorationManager for explore units. */
	virtual void setGoal(TilePosition goal);

	/** Clears the goal for this unit. */
	virtual void clearGoal();

	/** Returns the current goal for this unit. */
	TilePosition getGoal();

	/** Returns true if an enemy unit of the specified type is within maxRange of the agent. */
	bool isWithinRange(UnitType enemyUnitType, int maxRange);

	/** Returns the number of own units that are within maxRange and is of specified type of the agent. */
	int friendlyUnitsOfTypeWithinRange(UnitType type, int maxRange);

	/** Returns the number of own units that are within 6 tiles range of the agent. */
	int friendlyUnitsWithinRange();

	/** Returns the number of own units that are within maxRange of the agent. */
	int friendlyUnitsWithinRange(int maxRange);

	/** Returns the number of own units that are within maxRange of the specified tile. */
	int friendlyUnitsWithinRange(TilePosition tilePos, int maxRange);

	/** Returns the number of own units attacking the specified target unit. */
	int noUnitsAttacking(Unit* target);

	/** Returns the number of own melee units (Zealot, Dark Templars etc) 
	 * attacking the specified target unit. */
	int noMeleeUnitsAttacking(Unit* target);

	/** Returns a list of own units that are within maxRange of the agent. */
	vector<BaseAgent*> friendlyUnitsWithinRangeList(int maxRange);

	/** Returns the number of enemy units and buildings that can attack and are within firerange of the agent. */
	int enemyAttackingUnitsWithinRange();

	/** Returns the number of enemy units and buildings that can attack and are within range of the center tile. */
	int enemyAttackingUnitsWithinRange(int maxRange, TilePosition center);

	/** Returns the number of enemy units and buildings that can attack and are within firerange of the specified unit type. */
	int enemyAttackingUnitsWithinRange(UnitType type);

	/** Returns the number of enemy units and buildings that are within maxRange of the agent. */
	int enemyUnitsWithinRange(int maxRange);

	/** Returns the number of enemy ground units and buildings that are within maxRange of the agent. */
	int enemyGroundUnitsWithinRange(int maxRange);

	/** Returns the number of enemy ground units and buildings that are within maxRange of the center tile, and can attack the agent. */
	int enemyGroundAttackingUnitsWithinRange(TilePosition center, int maxRange);

	/** Returns the number of flying unit that are within maxRange of the agent. */
	int enemyAirUnitsWithinRange(int maxRange);

	/** Returns the number of flying unit that are within maxRange of the center tile, and can attack the agent. */
	int enemyAirAttackingUnitsWithinRange(TilePosition center, int maxRange);

	/** Returns a vector of all enemy units and buildings that are within maxRange of the agent. */
	vector<Unit*> enemyUnitsWithinRangeList(int maxRange);

	/** Returns the max firerange of the ground weapon this agent has, or -1 if it cannot attack ground. */
	int getGroundRange();

	/** Returns the max firerange of the ground weapon a unit of the specified type has, or -1 if it cannot attack ground. */
	static int getGroundRange(UnitType type);

	/** Returns true if the attacker UnitType can attack target UnitType. Note: Does not take spells into account, only weapons. */
	static bool canAttack(UnitType attacker, UnitType target);

	/** Returns the max firerange of the air weapon this agent has, or -1 if it cannot attack air. */
	int getAirRange();

	/** Returns the max firerange of the ground weapon a unit of the specified type has, or -1 if it cannot attack air. */
	static int getAirRange(UnitType type);

	/** Assigns this agent to explore the game world. */
	void assignToExplore();

	/** Assigns this agent to be in the attack force. */
	void assignToAttack();

	/** Assigns this agent to defend the base. */
	void assignToDefend();

	/** Returns true if this agent is used to explore the game world. */
	bool isExploring();

	/** Returns true if this agent is used to attack the enemy. */
	bool isAttacking();

	/** Returns true if this agent is defending the own base. */
	bool isDefending();

	/** Returns true if this agent is inside a Terran Bunker. */
	bool isBunkered();
	
	/** Returns the closest attacking enemy unit within maxRange, or NULL if not found. */
	Unit* getClosestAttackingEnemy(int maxRange);

	/** Returns the closest organic enemy unit within maxRange, or NULL if not found. */
	Unit* getClosestOrganicEnemy(int maxRange);

	/** Returns the closest enemy unit that is shielded and within maxRange, or NULL if not found. */
	Unit* getClosestShieldedEnemy(int maxRange);

	/** Returns the closest enemy turret or other attacking building within maxRange, or NULL if not found. */
	Unit* getClosestEnemyTurret(int maxRange);

	/** Returns the closest enemy turret, other attacking building or mechanical unit within maxRange that
	 * can attack air units, or NULL if not found. */
	Unit* getClosestEnemyAirDefense(int maxRange);

	/** Returns a list of all friendly bunkers. Terrans only. */
	vector<BaseAgent*> getBunkers();

	/** Used to print info about this agent to the screen. */
	virtual void printInfo();
};

#endif