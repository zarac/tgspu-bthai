#include "MedicAgent.h"
#include "PFManager.h"
#include "AgentManager.h"

MedicAgent::MedicAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("MedicAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;

	goal = TilePosition(-1, -1);
}

void MedicAgent::computeActions() {
	int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastFrame < 20) {
		return;
	}
	lastFrame = cFrame;

	if (checkUnitsToHeal()) {
		return;
	}
	
	//TODO: Add unitspecific code here

	PFManager::getInstance()->computeAttackingUnitActions(this, goal, true);
}

bool MedicAgent::checkUnitsToHeal() {
	try {
		double bestDist = -1;
		Unit* toHeal = NULL;

		AgentManager* agentManager = AgentManager::getInstance();
		for (int i = 0; i < (int)agentManager->size(); i++) {
			BaseAgent* agent = agentManager->at(i);
			if (agent->isAlive() && agent->isDamaged()) {
				if (isMedicTarget(agent->getUnit()) && agent->getUnitID() != unit->getID()) {
					Unit* cUnit = agent->getUnit();
					if (cUnit->exists() && cUnit->getHitPoints() > 0) {
						double dist = unit->getDistance(cUnit);
						if (bestDist < 0 || dist < bestDist) {
							bestDist = dist;
							toHeal = cUnit;
						}
					}
				}
			}
			AgentManager::release(agent);
		}

		if (bestDist >= 0 && toHeal !=NULL) {
			//Broodwar->printf("[%d] Medic healing", unitID);
			unit->useTech(TechTypes::Healing, toHeal);
		}
	}
	catch(exception) {
		Broodwar->printf("[%d] checkUnitToHeal() error", unit->getID());
	}

	return false;
}

bool MedicAgent::checkUnitsToFollow() {
	double bestDist = -1;
	Unit* toFollow = NULL;

	/*vector<BaseAgent*> agents = AgentManager::getInstance()->getAgents();
	for (int i = 0; i < (int)agents.size(); i++) {
		if (agents.at(i)->isAlive()) {
			Unit* cUnit = agents.at(i)->getUnit();
			if (isMedicTarget(cUnit)) {
				double dist = unit->getDistance(cUnit);
				if (bestDist < 0 || dist < bestDist) {
					bestDist = dist;
					toFollow = cUnit;
				}
			}
		}
	}*/
	for(set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++) {
		if (isMedicTarget((*i))) {
			double dist = unit->getDistance((*i));
			if (bestDist < 0 || dist < bestDist) {
				bestDist = dist;
				toFollow = (*i);
			}
		}
	}

	if (bestDist >= 128) {
		unit->follow(toFollow);
		return true;
	}
	else if (bestDist >= 0) {
		unit->stop();
		return true;
	}

	return false;
}

bool MedicAgent::isMedicTarget(Unit* mUnit) {
	if (!mUnit->getType().isOrganic()) {
		//Can only heal organic units
		return false;
	}

	if (mUnit->getType().isWorker()) {
		//We can heal workers, but no point
		//in following them
		return false;
	}

	if (!mUnit->getType().canAttack()) {
		//Dont follow units that cant attack
		return false;
	}

	if (isOfType(mUnit, UnitTypes::Terran_Medic)) {
		//Dont follow other medics
		return false;
	}

	if (mUnit->isLoaded()) {
		//Dont "follow" bunkered units
		return false;
	}

	//Check if the unit is exploring, then dont follow it.
	BaseAgent* agent = AgentManager::getInstance()->getAgent(mUnit->getID());
	if (agent->isExploring()) {
		return false;
	}
	return true;
}

string MedicAgent::getTypeName() {
	return "MedicAgent";
}
