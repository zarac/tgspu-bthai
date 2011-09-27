#ifndef __COVERMAP_H__
#define __COVERMAP_H__

#include "UnitAgent.h"

using namespace BWAPI;
using namespace std;

struct Corners {
	int x1;
	int y1;
	int x2;
	int y2;
};

/** The CoverMap class is used to keep track of the own base and which Tiles that are occupied by buildings,
 * and which Tiles are free and possibly can be used to construct new buildings on. 
 *
 * Internally a matrix of the same size as the map is used. If a Tile is occupied or cant be reached by ground
 * units, the value if the tile is 0. If the Tile can be built on, the value is 1. 
 * Buildings typically use up more space in the matrix than their actual size since we want some free space
 * around each building. Different types of buildings have different space requirements.
 *
 * The CoverMap is implemented as a singleton class. Each class that needs to access CoverMap can request an instance,
 * and all classes shares the same CoverMap instance.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class CoverMap {

private:
	CoverMap();
	static CoverMap* instance;
	static bool instanceFlag;

	int range;
	int w;
	int h;
	int** cover_map;

	Corners getCorners(Unit* unit);
	Corners getCorners(UnitType type, TilePosition center);

	void fill(Corners c);
	void clear(Corners c);
	
	Unit* findWorker();

public:
	~CoverMap();

	/** Returns the instance of the class. */
	static CoverMap* getInstance();

	/** Adds a newly constructed building to the cover map. */
	void addConstructedBuilding(Unit* unit);

	/** Called when a building is destroyed, to free up the space. */
	void buildingDestroyed(Unit* unit);

	/** Checks if the specified building type can be built at the buildSpot. True if it can,
	 * false otherwise. */
	bool canBuild(UnitType toBuild, TilePosition buildSpot);

	/** Blocks a position from being used as a valid buildSpot. Used when a worker is timedout when
	 * moving towards the buildSpot. */
	void blockPosition(TilePosition buildSpot);

	/** Finds and returns a buildSpot for the specified building type. Search for a buildSpot starts
	 * at the specified start position. If no buildspot is found, a TilePosition(-1,-1) is returned. */
	TilePosition findBuildSpot(UnitType toBuild, TilePosition start);

	/** Searches for the closest vespene gas that is not in use. If no gas is sighted,
	 * the ExplorationManager is queried. */
	TilePosition findRefineryBuildSpot(UnitType toBuild, TilePosition start);

	/** Finds and returns the position of the closest free vespene gas around the specified start position.
	 * If no gas vein is found, a TilePosition(-1, -1) is returned. */
	TilePosition findClosestGasWithoutRefinery(UnitType toBuild, TilePosition start);
};

#endif