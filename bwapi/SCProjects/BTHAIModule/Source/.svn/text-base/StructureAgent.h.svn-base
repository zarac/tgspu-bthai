#ifndef __STRUCTUREAGENT_H__
#define __STRUCTUREAGENT_H__

#include "BaseAgent.h"
using namespace BWAPI;
using namespace std;

/** The StructureAgent is the base agent class for all agents handling buildings. If a building is created and no
 * specific agent for that type is found, the building is assigned to a StructureAgent. StructureAgents are typically
 * agents without logic, for example supply depots. To add logic to a building, for example Terran Academy researching
 * stim packs, an agent implementation for that unit type must be created.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class StructureAgent : public BaseAgent {

private:
	
protected:
	bool repairing;
	vector<UpgradeType> upgradeOrder;
	vector<TechType> researchOrder;

public:
	StructureAgent();
	StructureAgent(Unit* mUnit);

	/** Called each update to issue orders. */
	virtual void computeActions() {
	}

	/** Returns true if this agent can research the specified tech. */
	bool canResearch(TechType type);

	/** Returns true if this agent can upgrade the specified upgradetype. */
	bool canUpgrade(UpgradeType type);

	/** Executes research, meaning that all techs in the researchOrder list are performed once
	 * enough resources are available. */
	void doResearch();

	/** Executes upgrades, meaning that all upgrades in the upgradeOrder list are performed once
	 * enough resources are available. */
	void doUpgrades();

	/** Returns the unique type name for structure agents. */
	string getTypeName();

	/** Used to print info about this agent to the screen. */
	virtual void printInfo();
};

#endif