#include "ScienceVesselAgent.h"
#include "PFManager.h"
#include "AgentManager.h"

ScienceVesselAgent::ScienceVesselAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("ScienceVesselAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;
	lastIrradiateFrame = 0;

	goal = TilePosition(-1, -1);
}

void ScienceVesselAgent::computeActions() {
	int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastFrame < 20) {
		return;
	}
	lastFrame = cFrame;

	//TODO: Add unitspecific code here

	bool defensive = true;	
	PFManager::getInstance()->computeAttackingUnitActions(this, goal, defensive);
}

BaseAgent* ScienceVesselAgent::findImportantUnit() {
	AgentManager* agentManager = AgentManager::getInstance();
	for (int i = 0; i < (int)agentManager->size(); i++) {
		BaseAgent* agent = agentManager->at(i);
		if (agent->isAlive() && isImportantUnit(agent) && !agent->getUnit()->isDefenseMatrixed()) {
			double dist = unit->getDistance(agent->getUnit());
			if (dist <= 320) {
				AgentManager::release(agent);
				return agent;
			}
		}
		AgentManager::release(agent);
	}
	return NULL;
}

bool ScienceVesselAgent::isImportantUnit(BaseAgent* agent) {
	UnitType type = agent->getUnit()->getType();

	if (agent->isOfType(UnitTypes::Terran_Siege_Tank_Tank_Mode)) return true;
	if (agent->isOfType(UnitTypes::Terran_Siege_Tank_Siege_Mode)) return true;
	if (agent->isOfType(UnitTypes::Terran_Science_Vessel)) return true;
	if (agent->isOfType(UnitTypes::Terran_Battlecruiser)) return true;

	return false;
}

string ScienceVesselAgent::getTypeName() {
	return "ScienceVesselAgent";
}
