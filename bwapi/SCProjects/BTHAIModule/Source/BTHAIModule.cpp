#define DISABLE_FOW 1

#include "BTHAIModule.h"
#include "BuildPlanner.h"
#include "SquadCommander.h"
#include "ExplorationManager.h"
#include "CoverMap.h"
#include "Commander.h"
using namespace BWAPI;

bool analyzed;
bool analysis_just_finished;
BWTA::Region* home;
BWTA::Region* enemy_base;

void BTHAIModule::onStart() {
	nShow_target_lines = 0;

	std::string VERSION = "1.4";

	Broodwar->printf("BTHAI bot version %s started playing %s", VERSION.c_str(), Broodwar->self()->getRace().getName().c_str());
	Broodwar->printf("The map is %s, a %d player map", Broodwar->mapName().c_str(),Broodwar->getStartLocations().size());
	
	//Needed for BWAPI to work
	Broodwar->enableFlag(Flag::UserInput);
	//Set max speed
	speed = 10; //0 = fastest speed possible
	Broodwar->setLocalSpeed(speed);
	//Uncomment to enable complete map information
#if DISABLE_FOW == 1
	Broodwar->enableFlag(Flag::CompleteMapInformation);
	ExplorationManager::getInstance()->setInactive();
#endif

	statistics = new Statistics();

	//read map information into BWTA so terrain analysis can be done in another thread
	BWTA::readMap();
	analyzed=false;
	analysis_just_finished=false;

	//Analyze map using BWTA
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL);

	show_bullets=false;
	show_visibility_data=false;
	show_terrain_data=false;
	show_goals=false;

	BuildPlanner::getInstance();

	if (Broodwar->isReplay()) {
		Broodwar->printf("The following players are in this replay:");
		for(std::set<Player*>::iterator p=Broodwar->getPlayers().begin();p!=Broodwar->getPlayers().end();p++) {
			if (!(*p)->getUnits().empty() && !(*p)->isNeutral()) {
				Broodwar->printf("%s, playing as a %s",(*p)->getName().c_str(),(*p)->getRace().getName().c_str());
			}
		}
	}
	
    //Add the units we have from start to agent manager
	for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++) {
		AgentManager::getInstance()->addAgent(*i);
	}

	CoverMap::getInstance();

	running = true;
}

void BTHAIModule::onEnd(bool isWinner) {
	Broodwar->printf("Game ended");
	if (isWinner) {
		Broodwar->printf("BTHAI won");
	}
	else {
		Broodwar->printf("BTHAI lost");
	}
	statistics->WriteStatisticsFile(isWinner);
	delete(statistics);
	running = false;
}

void BTHAIModule::onFrame() {
	if (!analyzed) {
		if (Broodwar->getFrameCount() % 200 == 0) {
			Broodwar->printf("Still analyzing map");
		}
		return;
	}
	if (!running) {
		//Game over. Do nothing
		return;
	}
	if (!Broodwar->isInGame()) {
		//Not in game. Do nothing
		return;
	}

	if (show_visibility_data) {
		drawVisibilityData();
	}
	if (show_bullets) {
		drawBullets();
	}
	if (show_terrain_data) {
		drawTerrainData();
	}
	if (show_goals) {
		showGoals();
	}

	if (Broodwar->isReplay()) {
		return;
	}

	//drawEnemyStats();

	// Draw some lines if user choosed to.
	if (nShow_target_lines == 1 || nShow_target_lines == 2 && Broodwar->getFrameCount() % 2 == 0) {
		AgentManager* agentManger = AgentManager::getInstance();
		for (int i = 0; i < (int)agentManger->size(); i++) {
			BaseAgent* agent = agentManger->at(i);
			if (agent->isAlive()) {
				Unit* unit = agent->getUnit();
				if(unit == NULL)
					continue;
				Color lineColor = Color(BWAPI::Colors::Green);
				Order order = unit->getOrder();
				if(order.getName().substr(0, 6).compare("Attack") == 0)
					lineColor = Color(BWAPI::Colors::Red);
				Broodwar->drawLine(CoordinateType::Map, unit->getPosition().x(), unit->getPosition().y(), unit->getTargetPosition().x(), unit->getTargetPosition().y(), lineColor);
			}
			AgentManager::release(agent);
		}
		if (analyzed)
			drawTerrainData();
	}

	//TODO: TEST
	//ExplorationManager::getInstance()->debug();

	if (Broodwar->getFrameCount() % 10 == 0) {
		AgentManager::getInstance()->computeActions();
	}

	if (Broodwar->getFrameCount() % 25 == 0) {
		BuildPlanner::getInstance()->computeActions();
	}

	/*if (Broodwar->getFrameCount() % 25 == 0) {
		SquadCommander::getInstance()->computeActions();
	}*/

	if (Broodwar->getFrameCount() % 25 == 0) {
		Commander::getInstance()->computeActions();
	}

	if (Broodwar->getFrameCount() % 11 == 0) {
		ExplorationManager::getInstance()->computeActions();
	}

	if (Broodwar->getFrameCount() % 202 == 0) {
		AgentManager::getInstance()->cleanup();
	}
}

void BTHAIModule::onSendText(std::string text) {
	if (text=="/show bullets") {
		show_bullets = !show_bullets;
	} 
	else if (text=="/show players") {
		showPlayers();
	}
	else if (text=="/show forces") {
		showForces();
	}
	else if (text=="/show visibility") {
		show_visibility_data=!show_visibility_data;
	}
	else if (text=="/show lines") {
		++nShow_target_lines%=3;
	}
	else if (text=="/analyze") {
		if (analyzed == false) {
			Broodwar->printf("Analyzing map... this may take a minute");
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL);
		}
	}
	else if (text=="/ta") {
		show_terrain_data = !show_terrain_data;
	}
	else if(text=="/goals") {
		show_goals = !show_goals;
	}
	else if (text=="sc") {
		SquadCommander::getInstance()->printInfo();
	}
	else if (text.substr(0, 2)=="sq") {
		int id = atoi(&text[2]);
		Squad* squad = Commander::getInstance()->getSquad(id);
		if (squad != NULL) {
			squad->printFullInfo();
		}
	}
	else if (text=="bp") {
		BuildPlanner::getInstance()->printInfo();
	}
	else if (text=="e") {
		drawEnemyStats();
	}
	else if (text=="+") {
		speed -= 4;
		if (speed < 0) {
			speed = 0;
		}
		Broodwar->printf("Speed increased to %d", speed);
		Broodwar->setLocalSpeed(speed);
	}
	else if (text=="-") {
		speed += 4;
		Broodwar->printf("Speed decreased to %d", speed);
		Broodwar->setLocalSpeed(speed);
	}
	else if (text=="/mineral") {
		Position startPoint = Position(Broodwar->self()->getStartLocation());
		double bDist = -1;
		for(set<Unit*>::iterator m = Broodwar->getMinerals().begin(); m != Broodwar->getMinerals().end(); m++) {
			double cDist = (*m)->getDistance(startPoint);
			if (bDist < 0 ||  cDist < bDist) {
				bDist = cDist;
			}
		}
		if (bDist >= 0) {
			Broodwar->printf("Closest mineral is at distance %d", (int)bDist);
		}
	}
	else if (text=="i") {
		set<Unit*> units = Broodwar->getSelectedUnits();
		if ((int)units.size() > 0) {
			int unitID = (*units.begin())->getID();
			BaseAgent* agent = AgentManager::getInstance()->getAgent(unitID);
			if (agent != NULL) {
				agent->printInfo();
			}
			else {
				Unit* mUnit = (*units.begin());
				if (mUnit->getType().isNeutral()) {
					//Neutral unit. Check distance to base.
					BaseAgent* agent = AgentManager::getInstance()->getAgent(UnitTypes::Terran_Command_Center);
					double dist = agent->getUnit()->getDistance(mUnit);
					Broodwar->printf("Distance to base: %d", (int)dist);
				}
			}
			
		}
	}
	else {
		Broodwar->printf("You typed '%s'!",text.c_str());
	}
}

void BTHAIModule::onReceiveText(BWAPI::Player* player, std::string text) {
	Broodwar->printf("%s said '%s'", player->getName().c_str(), text.c_str());
}

void BTHAIModule::onPlayerLeft(BWAPI::Player* player) {
	Broodwar->sendText("%s left the game.",player->getName().c_str());
	running = false;
}

void BTHAIModule::onNukeDetect(BWAPI::Position target) {
	if (target != Positions::Unknown) {
		Broodwar->printf("Nuclear Launch Detected at (%d,%d)",target.x(),target.y());
	}
	else {
		Broodwar->printf("Nuclear Launch Detected");
	}
}

void BTHAIModule::onUnitDiscover(BWAPI::Unit* unit) {
	/*if (!Broodwar->isReplay() && Broodwar->getFrameCount()>1 && unit->getPlayer()->getID() != Broodwar->self()->getID()) {
		Broodwar->sendText("A %s [%x] has been discovered at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
	}*/
}

void BTHAIModule::onUnitEvade(BWAPI::Unit* unit) {
	/*if (!Broodwar->isReplay() && Broodwar->getFrameCount()>1 && unit->getPlayer()->getID() != Broodwar->self()->getID()) {
		Broodwar->sendText("A %s [%x] was last accessible at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
	}*/
}

void BTHAIModule::onUnitShow(BWAPI::Unit* unit) {
	/*if (!Broodwar->isReplay() && Broodwar->getFrameCount()>1 && unit->getPlayer()->getID() != Broodwar->self()->getID()) {
		Broodwar->sendText("A %s [%x] has been spotted at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
	}*/
	if (!Broodwar->isReplay() && Broodwar->getFrameCount()>1 && unit->getPlayer()->getID() != Broodwar->self()->getID()) {
		if (!unit->getPlayer()->isNeutral() && !unit->getPlayer()->isAlly(Broodwar->self())) {
			ExplorationManager::getInstance()->addSpottedUnit(unit);
		}
	}
	if (unit->getType().isResourceContainer()) {
		ExplorationManager::getInstance()->addSpottedResource(unit);
	}
}

void BTHAIModule::onUnitHide(BWAPI::Unit* unit) {
	/*if (!Broodwar->isReplay() && Broodwar->getFrameCount()>1 && unit->getPlayer()->getID() != Broodwar->self()->getID()) {
		Broodwar->sendText("A %s [%x] was last seen at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
	}*/
}

void BTHAIModule::onUnitCreate(BWAPI::Unit* unit)
{
	if (Broodwar->getFrameCount()>1) {
		if (!Broodwar->isReplay()) {
			//Broodwar->sendText("A %s [%x] has been created at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
			if (unit->getPlayer()->getID() == Broodwar->self()->getID()) {
				AgentManager::getInstance()->addAgent(unit);
				BuildPlanner::getInstance()->unlock(unit->getType());
			}
		}
		else {
			/*if we are in a replay, then we will print out the build order
			(just of the buildings, not the units).*/
			if (unit->getType().isBuilding() && unit->getPlayer()->isNeutral() == false) {
				int seconds=Broodwar->getFrameCount()/24;
				int minutes=seconds/60;
				seconds%=60;
				Broodwar->sendText("%.2d:%.2d: %s creates a %s",minutes,seconds,unit->getPlayer()->getName().c_str(),unit->getType().getName().c_str());
			}
		}
	}
}

void BTHAIModule::onUnitDestroy(BWAPI::Unit* unit)
{
	if (!Broodwar->isReplay() && Broodwar->getFrameCount()>1 && unit->getPlayer()->getID() != Broodwar->self()->getID()) {
		ExplorationManager::getInstance()->unitDestroyed(unit);
	}
	if (!Broodwar->isReplay() && Broodwar->getFrameCount()>1 && unit->getPlayer()->getID() == Broodwar->self()->getID()) {
		AgentManager::getInstance()->removeAgent(unit);
		if (unit->getType().isBuilding()) {
			BuildPlanner::getInstance()->buildingDestroyed(unit);
		}
	}
}

void BTHAIModule::onUnitMorph(BWAPI::Unit* unit)
{
	if (!Broodwar->isReplay()) {
		//Broodwar->sendText("A %s [%x] has been morphed at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
		if (unit->getPlayer()->getID() == Broodwar->self()->getID()) {
			AgentManager::getInstance()->addAgent(unit);
			BuildPlanner::getInstance()->unlock(unit->getType());
		}
	}
	else {
		/*if we are in a replay, then we will print out the build order
		(just of the buildings, not the units).*/
		if (unit->getType().isBuilding() && unit->getPlayer()->isNeutral() == false) {
			int seconds=Broodwar->getFrameCount()/24;
			int minutes=seconds/60;
			seconds%=60;
			Broodwar->sendText("%.2d:%.2d: %s morphs a %s",minutes,seconds,unit->getPlayer()->getName().c_str(),unit->getType().getName().c_str());
		}
	}
}

void BTHAIModule::onUnitRenegade(BWAPI::Unit* unit) {
	/*if (!Broodwar->isReplay()) {
		Broodwar->sendText("A %s [%x] is now owned by %s",unit->getType().getName().c_str(),unit,unit->getPlayer()->getName().c_str());
	}*/
}

void BTHAIModule::onSaveGame(std::string gameName) {
	Broodwar->printf("The game was saved to \"%s\".", gameName.c_str());
}

DWORD WINAPI AnalyzeThread()
{
	Broodwar->printf("Start analyzing map");
	BWTA::analyze();
	Broodwar->printf("Done analyzing");

	//self start location only available if the map has base locations
	if (BWTA::getStartLocation(BWAPI::Broodwar->self()) != NULL) {
		home = BWTA::getStartLocation(BWAPI::Broodwar->self())->getRegion();
	}
	//enemy start location only available if Complete Map Information is enabled.
	if (BWTA::getStartLocation(BWAPI::Broodwar->enemy())!=NULL) {
		enemy_base = BWTA::getStartLocation(BWAPI::Broodwar->enemy())->getRegion();
	}

	TilePosition myStart = Broodwar->self()->getStartLocation();
	for(set<BWTA::BaseLocation*>::const_iterator i=BWTA::getStartLocations().begin();i!=BWTA::getStartLocations().end();i++) {
		TilePosition sLoc = (*i)->getTilePosition();
		double dist = sLoc.getDistance(myStart);
		if (dist >= 10) {
			ExplorationManager::getInstance()->addSpottedStartLocation(sLoc);
		}
	}

	analyzed = true;
	analysis_just_finished = true;
	return 0;
}

void BTHAIModule::drawOwnStats() {
	std::set<Unit*> myUnits = Broodwar->self()->getUnits();
	Broodwar->drawTextScreen(5,0,"I have %d units:",myUnits.size());
	std::map<UnitType, int> unitTypeCounts;
	for(std::set<Unit*>::iterator i=myUnits.begin();i!=myUnits.end();i++) {
		if (unitTypeCounts.find((*i)->getType())==unitTypeCounts.end()) {
			unitTypeCounts.insert(std::make_pair((*i)->getType(),0));
		}
		unitTypeCounts.find((*i)->getType())->second++;
	}
	int line=1;

	for(std::map<UnitType,int>::iterator i=unitTypeCounts.begin();i!=unitTypeCounts.end();i++) {
		Broodwar->drawTextScreen(5,16*line,"- %d %ss",(*i).second, (*i).first.getName().c_str());
		line++;
	}
}

void BTHAIModule::drawEnemyStats() {
	std::set<Unit*> myUnits = Broodwar->enemy()->getUnits();
	Broodwar->drawTextScreen(5,0,"I have %d units:",myUnits.size());
	std::map<UnitType, int> unitTypeCounts;
	for(std::set<Unit*>::iterator i=myUnits.begin();i!=myUnits.end();i++) {
		if (unitTypeCounts.find((*i)->getType())==unitTypeCounts.end()) {
			unitTypeCounts.insert(std::make_pair((*i)->getType(),0));
		}
		unitTypeCounts.find((*i)->getType())->second++;
	}
	int line=1;

	for(std::map<UnitType,int>::iterator i=unitTypeCounts.begin();i!=unitTypeCounts.end();i++) {
		Broodwar->drawTextScreen(5,16*line,"- %d %ss",(*i).second, (*i).first.getName().c_str());
		line++;
	}
}

void BTHAIModule::drawBullets()
{
	std::set<Bullet*> bullets = Broodwar->getBullets();
	for(std::set<Bullet*>::iterator i=bullets.begin();i!=bullets.end();i++) {
		Position p=(*i)->getPosition();
		double velocityX = (*i)->getVelocityX();
		double velocityY = (*i)->getVelocityY();
		if ((*i)->getPlayer()==Broodwar->self()) {
			Broodwar->drawLineMap(p.x(),p.y(),p.x()+(int)velocityX,p.y()+(int)velocityY,Colors::Green);
			Broodwar->drawTextMap(p.x(),p.y(),"\x07%s",(*i)->getType().getName().c_str());
		}
		else {
			Broodwar->drawLineMap(p.x(),p.y(),p.x()+(int)velocityX,p.y()+(int)velocityY,Colors::Red);
			Broodwar->drawTextMap(p.x(),p.y(),"\x06%s",(*i)->getType().getName().c_str());
		}
	}
}

void BTHAIModule::drawVisibilityData() {
	for(int x=0;x<Broodwar->mapWidth();x++) {
		for(int y=0;y<Broodwar->mapHeight();y++) {
			if (Broodwar->isExplored(x,y)) {
				if (Broodwar->isVisible(x,y))
					Broodwar->drawDotMap(x*32+16,y*32+16,Colors::Green);
				else
					Broodwar->drawDotMap(x*32+16,y*32+16,Colors::Blue);
			}
			else
				Broodwar->drawDotMap(x*32+16,y*32+16,Colors::Red);
		}
	}
}

void BTHAIModule::drawTerrainData() {
	//we will iterate through all the base locations, and draw their outlines.
	for(std::set<BWTA::BaseLocation*>::const_iterator i=BWTA::getBaseLocations().begin();i!=BWTA::getBaseLocations().end();i++) {
		TilePosition p=(*i)->getTilePosition();
		Position c=(*i)->getPosition();

		//draw outline of center location
		Broodwar->drawBox(CoordinateType::Map,p.x()*32,p.y()*32,p.x()*32+4*32,p.y()*32+3*32,Colors::Blue,false);

		//draw a circle at each mineral patch
		for(std::set<BWAPI::Unit*>::const_iterator j=(*i)->getStaticMinerals().begin();j!=(*i)->getStaticMinerals().end();j++) {
			Position q=(*j)->getInitialPosition();
			Broodwar->drawCircle(CoordinateType::Map,q.x(),q.y(),30,Colors::Cyan,false);
		}

		//draw the outlines of vespene geysers
		for(std::set<BWAPI::Unit*>::const_iterator j=(*i)->getGeysers().begin();j!=(*i)->getGeysers().end();j++) {
			TilePosition q=(*j)->getInitialTilePosition();
			Broodwar->drawBox(CoordinateType::Map,q.x()*32,q.y()*32,q.x()*32+4*32,q.y()*32+2*32,Colors::Orange,false);
		}

		//if this is an island expansion, draw a yellow circle around the base location
		if ((*i)->isIsland())
			Broodwar->drawCircle(CoordinateType::Map,c.x(),c.y(),80,Colors::Yellow,false);
	}

	//we will iterate through all the regions and draw the polygon outline of it in green.
	for(std::set<BWTA::Region*>::const_iterator r=BWTA::getRegions().begin();r!=BWTA::getRegions().end();r++) {
		BWTA::Polygon p=(*r)->getPolygon();
		for(int j=0;j<(int)p.size();j++) {
			Position point1=p[j];
			Position point2=p[(j+1) % p.size()];
			Broodwar->drawLine(CoordinateType::Map,point1.x(),point1.y(),point2.x(),point2.y(),Colors::Green);
		}
	}

	//we will visualize the chokepoints with red lines
	for(std::set<BWTA::Region*>::const_iterator r=BWTA::getRegions().begin();r!=BWTA::getRegions().end();r++) {
		for(std::set<BWTA::Chokepoint*>::const_iterator c=(*r)->getChokepoints().begin();c!=(*r)->getChokepoints().end();c++) {
			Position point1=(*c)->getSides().first;
			Position point2=(*c)->getSides().second;
			Broodwar->drawLine(CoordinateType::Map,point1.x(),point1.y(),point2.x(),point2.y(),Colors::Red);
		}
	}
}

void BTHAIModule::showPlayers() {
	std::set<Player*> players=Broodwar->getPlayers();
	for(std::set<Player*>::iterator i=players.begin();i!=players.end();i++) {
		Broodwar->printf("Player [%d]: %s is in force: %s",(*i)->getID(),(*i)->getName().c_str(), (*i)->getForce()->getName().c_str());
	}
}

void BTHAIModule::showForces() {
	std::set<Force*> forces=Broodwar->getForces();
	for(std::set<Force*>::iterator i=forces.begin();i!=forces.end();i++) {
		std::set<Player*> players=(*i)->getPlayers();
		Broodwar->printf("Force %s has the following players:",(*i)->getName().c_str());
		for(std::set<Player*>::iterator j=players.begin();j!=players.end();j++) {
			Broodwar->printf("  - Player [%d]: %s",(*j)->getID(),(*j)->getName().c_str());
		}
	}
}

void BTHAIModule::showGoals() {
	AgentManager* agentManager = AgentManager::getInstance();
	for (int i = 0; i < (int)agentManager->size(); i++) {
		BaseAgent* agent = agentManager->at(i);
		if (agent->isAlive()) {
			Unit* u = agent->getUnit();
			
			bool show = true;
			if (u->getType().isBuilding()) show = false;
			if (u->isLoaded()) show = false;
			if (BaseAgent::isOfType(u, UnitTypes::Terran_Vulture_Spider_Mine)) show = false;
			if (u->getType().isWorker()) show = false;
			if (agent->getGoal().x() < 0) show = false;

			if (show) {
				Position a = Position(agent->getUnit()->getTilePosition());
				Position b = Position(agent->getGoal());
				Broodwar->drawLine(CoordinateType::Map,a.x(),a.y(),b.x(),b.y(),Colors::Yellow);
			}
		}
		AgentManager::release(agent);
	}
}
