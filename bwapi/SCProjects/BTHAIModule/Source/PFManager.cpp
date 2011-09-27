#include "PFManager.h"
#include "PFFunctions.h"
#include "UnitAgent.h"
#include "AgentManager.h"
#include "ExplorationManager.h"
#include "Commander.h"

bool PFManager::instanceFlag = false;
PFManager* PFManager::instance = NULL;

PFManager::PFManager() {
	checkRange = 96;
	stepSize = 32;
	mapW = Broodwar->mapWidth() * 32;
	mapH = Broodwar->mapHeight() * 32;
}

PFManager::~PFManager() {
	instanceFlag = false;
	delete instance;
}

PFManager* PFManager::getInstance() {
	if (!instanceFlag) {
		instance = new PFManager();
		instanceFlag = true;
	}
	return instance;
}

void PFManager::computeMedicUnitActions(BaseAgent* agent) {
	Unit* unit = agent->getUnit();

	int unitX = unit->getPosition().x();
	int unitY = unit->getPosition().y();

	float bestP = getMedicUnitP(agent, unitX, unitY);
	float cP = 0;
	
	float startP = bestP;
	int bestX = -1;
	int bestY = -1;

	for (int cX = unitX - checkRange; cX <= unitX + checkRange; cX += stepSize) {
		for (int cY = unitY - checkRange; cY <= unitY + checkRange; cY += stepSize) {
			if (cX >= 0 && cY >= 0 && cX <= mapW && cY <= mapH) {
				cP = getMedicUnitP(agent, cX, cY);
				
				if (cP > bestP) {
					bestP = cP;
					bestX = cX;
					bestY = cY;
				}
			}
		}
	}

	if (bestX >= 0 && bestY >= 0)
	{
		Position toMove(bestX, bestY);
		unit->rightClick(toMove);
	}
}

void PFManager::computeAttackingUnitActions(BaseAgent* agent, TilePosition goal, bool defensive) {
	Unit* unit = agent->getUnit();

	int unitX = unit->getPosition().x();
	int unitY = unit->getPosition().y();

	float bestP = getAttackingUnitP(agent, unitX, unitY, defensive);
	float cP = 0;
	
	float startP = bestP;
    int bestX = -1;
    int bestY = -1;

	for (int cX = unitX - checkRange; cX <= unitX + checkRange; cX += stepSize) {
        for (int cY = unitY - checkRange; cY <= unitY + checkRange; cY += stepSize) {
            if (cX >= 0 && cY >= 0 && cX <= mapW && cY <= mapH) {
				cP = getAttackingUnitP(agent, cX, cY, defensive);
				//Broodwar->printf("p(%d,%d)=%d",cX,cY,cP);

                if (cP > bestP) {
					bestP = cP;
                    bestX = cX;
                    bestY = cY;
                }
            }
        }
    }

	if (bestX >= 0 && bestY >= 0)
    {
		//Broodwar->printf("Move to (%d,%d)", bestX, bestY);
		//Broodwar->printf("p(start)=%d", (int)startP);
		//Broodwar->printf("p(new)=%d", (int)bestP);
		Position toMove(bestX, bestY);
		if (!defensive) {
			//Offensive -> attackMove
			agent->setInfo("PF attack move");
			unit->attackMove(toMove);
		}
		else {
			//Defensive -> rightClick
			agent->setInfo("PF rightclick move");
			unit->rightClick(toMove);
		}
    }
	else if (goal.x() != -1) {
		moveToGoal(agent, goal);
	}
}

bool PFManager::moveToGoal(BaseAgent* agent, TilePosition goal) {
	Unit* unit = agent->getUnit();

	//TODO: Testing this
	//Idea is good, but doesnt work well atm.
	Squad* mSquad = Commander::getInstance()->getSquad(agent->getSquadID());
	if (mSquad != NULL && mSquad->isActive() && mSquad->getSize() > 1) {
		TilePosition center = mSquad->getCenter();
		TilePosition cPos = agent->getUnit()->getTilePosition();
		double dist = center.getDistance(cPos);
		if (dist >= 8) {
			agent->getUnit()->rightClick(Position(center));
			return true;
		}
	}
	//

	//Code to make sure we dont crowd defense positions by letting too
	//many units move there.
	if (agent->isDefending() && !agent->isOfType(UnitTypes::Terran_Siege_Tank_Tank_Mode)) {
		int noBases = AgentManager::getInstance()->countNoBases();
		if (noBases <= 1) {
			UnitAgent* ua = (UnitAgent*)agent;
			int fCnt = ua->friendlyUnitsWithinRange(goal, 4*32);
			if (fCnt >= 14) {
				unit->stop();
				return true;
			}
		}
	}

	Position toReach = Position(goal);
	double distToReach = toReach.getDistance(unit->getPosition());

	if (agent->isDefending() && distToReach <= 64) {
		unit->stop();
		return true;
	}

	int engageDist = unit->getType().groundWeapon().maxRange();
	if (agent->isOfType(UnitTypes::Terran_Medic)) {
		engageDist = 3 * 32;
	}

	if (distToReach <= engageDist) {
		agent->setInfo("Engage");
		unit->stop();
		return true;
	}
	
	//Move
	agent->setInfo("AttackMove");
	return unit->attackMove(toReach);
}

float PFManager::getAttackingUnitP(BaseAgent* agent, int cX, int cY, bool defensive) {
	float p = 0;
	Position pos(cX, cY);

	//Enemy Units
	int eCnt = 0;
	for(set<Unit*>::const_iterator i=Broodwar->enemy()->getUnits().begin();i!=Broodwar->enemy()->getUnits().end();i++) {
		//Enemy seen
		float dist = PFFunctions::getDistance(pos, (*i));
		float ptmp = PFFunctions::calcAttackingUnitP(dist, agent->getUnit(), (*i), defensive);

        if (ptmp > p) {
            p = ptmp;
        }
	}

	//TODO: Test
	if (p == 0) {
		return 0;
	}
	//Broodwar->printf("[%d] e=%d", agent->getUnitID(), (int)p);

	//Own Units
	for(set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++) {
		float dist = PFFunctions::getDistance(pos, (*i));
		float ptmp = PFFunctions::calcOwnUnitP(dist, agent->getUnit(), (*i));

        p += ptmp;
	}

	//Neutral Units
	for(set<Unit*>::const_iterator i=Broodwar->getNeutralUnits().begin();i!=Broodwar->getNeutralUnits().end();i++) {
		float dist = PFFunctions::getDistance(pos, (*i));
		float ptmp = PFFunctions::calcMineP(dist, agent->getUnit());

        p += ptmp;
	}

	//Broodwar->printf("[%d] o=%d", agent->getUnitID(), (int)p);

	//Terrain
	if (!Broodwar->isWalkable(cX/8, cY/8)) {
		p -= 50;
	}

	//Broodwar->printf("[%d] t=%d", agent->getUnitID(), (int)p);

	return p;
}

float PFManager::getMedicUnitP(BaseAgent* agent, int cX, int cY) {
	float p = 0;
	Position pos(cX, cY);

	//Enemy Units
	int eCnt = 0;
	for(set<Unit*>::const_iterator i=Broodwar->enemy()->getUnits().begin();i!=Broodwar->enemy()->getUnits().end();i++) {
		//Enemy seen
		float dist = PFFunctions::getDistance(pos, (*i));
		float ptmp = PFFunctions::calcDefensiveUnitP(dist, agent->getUnit(), (*i));

        if (ptmp > p) {
            p = ptmp;
        }
	}
	
	//Own Units
	for(set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++) {
		float dist = PFFunctions::getDistance(pos, (*i));
		float ptmp = PFFunctions::calcOwnUnitMedicP(dist, agent->getUnit(), (*i));

        p += ptmp;
	}

	//Terrain
	if (!Broodwar->isWalkable(cX/8, cY/8)) {
		p -= 50;
	}

	return p;
}
