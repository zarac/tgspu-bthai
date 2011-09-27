#include "HighTemplarAgent.h"
#include "PFManager.h"
#include "AgentManager.h"

HighTemplarAgent::HighTemplarAgent(Unit* mUnit) {
	unit = mUnit;
	unitID = unit->getID();
	//Broodwar->printf("HighTemplarAgent created (%s)", unit->getType().getName().c_str());
	lastFrame = 0;

	goal = TilePosition(-1, -1);
	hasCastTransform = false;
}

void HighTemplarAgent::computeActions() {
	int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastFrame < 20) {
		return;
	}
	lastFrame = cFrame;

	bool defensive = false;

	if (isOfType(unit->getType(), UnitTypes::Protoss_Archon)) {
		//High Templar has been transformed to an Archon.
		defensive = false;
	}
	else {
		//High Templar unit, use spells
		defensive = true;
		
		//Out of Energy, see if we can transform to an Archon
		if (unit->getEnergy() <= 50 && enemyUnitsWithinRange(320) > 0) {
			if (!hasCastTransform) {
				BaseAgent* target = findArchonTarget();
				if (target != NULL) {
					unit->useTech(TechTypes::Archon_Warp, target->getUnit());
					hasCastTransform = true;
					Broodwar->printf("[%d] %s uses Summon Archon on %s", unitID, unit->getType().getName().c_str(), target->getUnit()->getType().getName().c_str());
					return;
				}
			}
		}
		
		//TODO: Add unitspecific code here
	}

	PFManager::getInstance()->computeAttackingUnitActions(this, goal, defensive);
}

BaseAgent* HighTemplarAgent::findArchonTarget() {
	int maxRange = 12 * 32;

	AgentManager* agentManager = AgentManager::getInstance();
	for (int i = 0; i < (int)agentManager->size(); i++) {
		BaseAgent* agent = agentManager->at(i);
		if (!agent->matches(unit) && agent->isAlive() && agent->isOfType(UnitTypes::Protoss_High_Templar) && agent->getUnit()->getEnergy() <= 50) {
			AgentManager::release(agent);
			return agent;
		}
		AgentManager::release(agent);
	}

	return NULL;
}

string HighTemplarAgent::getTypeName() {
	return "HighTemplarAgent";
}
