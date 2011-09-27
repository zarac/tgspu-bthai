#pragma once
#include "AgentManager.h"
#include "Statistics.h"

#include <BWTA.h>
#include <windows.h>
#include "BWTAExtern.h"

DWORD WINAPI AnalyzeThread();

/** This class contains the main game loop and all events that is broadcasted from the Starcraft engine
 * using BWAPI. See the BWAPI documentation for more info. 
 *
 * Author: Contained in BWAPI 3.0.3
 * Modified: Johan Hagelback (johan.hagelback@gmail.com)
 */
class BTHAIModule : public BWAPI::AIModule
{
private:
	bool running;
public:
	virtual void onStart();
	virtual void onEnd(bool isWinner);
	virtual void onFrame();
	virtual void onSendText(std::string text);
	virtual void onReceiveText(BWAPI::Player* player, std::string text);
	virtual void onPlayerLeft(BWAPI::Player* player);
	virtual void onNukeDetect(BWAPI::Position target);
	virtual void onUnitDiscover(BWAPI::Unit* unit);
	virtual void onUnitEvade(BWAPI::Unit* unit);
	virtual void onUnitShow(BWAPI::Unit* unit);
	virtual void onUnitHide(BWAPI::Unit* unit);
	virtual void onUnitCreate(BWAPI::Unit* unit);
	virtual void onUnitDestroy(BWAPI::Unit* unit);
	virtual void onUnitMorph(BWAPI::Unit* unit);
	virtual void onUnitRenegade(BWAPI::Unit* unit);
	virtual void onSaveGame(std::string gameName);

	//not part of BWAPI::AIModule
	void drawOwnStats(); 
	void drawEnemyStats();
	void drawBullets();
	void drawVisibilityData();
	void drawTerrainData();
	void showPlayers();
	void showForces();
	void showGoals();
	bool show_bullets;
	bool show_visibility_data;
	bool show_terrain_data;
	bool show_goals;
	/** 0 = Don't show lines, 1 = Show each frame, 2 = Show each second frame */
	int nShow_target_lines;
	int speed;
	Statistics* statistics;
};
