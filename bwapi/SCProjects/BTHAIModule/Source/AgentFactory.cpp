#include "AgentFactory.h"
#include "WorkerAgent.h"
#include "StructureAgent.h"
#include "UnitAgent.h"

/** Terran agents */
#include "CommandCenterAgent.h"
#include "BarracksAgent.h"
#include "FactoryAgent.h"
#include "EngineeringBayAgent.h"
#include "AcademyAgent.h"
#include "ArmoryAgent.h"
#include "MachineShopAgent.h"
#include "SiegeTankAgent.h"
#include "MarineAgent.h"
#include "MedicAgent.h"
#include "VultureAgent.h"
#include "StarportAgent.h"
#include "ScienceFacilityAgent.h"
#include "FirebatAgent.h"
#include "RefineryAgent.h"
#include "ScienceVesselAgent.h"
#include "BattlecruiserAgent.h"
#include "PhysicsLabAgent.h"
#include "ControlTowerAgent.h"
#include "WraithAgent.h"
#include "CovertOpsAgent.h"
#include "ComsatAgent.h"
#include "BunkerAgent.h"
#include "GhostAgent.h"
#include "GoliathAgent.h"
#include "ValkyrieAgent.h"
#include "DropshipAgent.h"

/** Protoss agents */
#include "NexusAgent.h"
#include "GatewayAgent.h"
#include "AssimilatorAgent.h"
#include "CyberneticsCoreAgent.h"
#include "ZealotAgent.h"
#include "DragoonAgent.h"
#include "RoboticsFacilityAgent.h"
#include "RoboticsSupportBayAgent.h"
#include "ReaverAgent.h"
#include "ObserverAgent.h"
#include "ObservatoryAgent.h"
#include "CitadelOfAdunAgent.h"
#include "TemplarArchivesAgent.h"
#include "DarkTemplarAgent.h"
#include "StargateAgent.h"
#include "ScoutAgent.h"
#include "ForgeAgent.h"
#include "CorsairAgent.h"
#include "FleetBeaconAgent.h"
#include "ArbiterTribunalAgent.h"
#include "ShieldBatteryAgent.h"
#include "HighTemplarAgent.h"

// ZERGZ0R
#include "HatcheryAgent.h"

bool AgentFactory::instanceFlag = false;
AgentFactory* AgentFactory::instance = NULL;

AgentFactory::AgentFactory() {
	
}

AgentFactory::~AgentFactory() {
	instanceFlag = false;
	delete instance;
}

AgentFactory* AgentFactory::getInstance() {
	if (!instanceFlag) {
		instance = new AgentFactory();
		instanceFlag = true;
	}
	return instance;
}

BaseAgent* AgentFactory::createAgent(Unit* unit) {
	if (Broodwar->self()->getRace().getID() == Races::Terran.getID()) {
		return createTerranAgent(unit);
	}
	if (Broodwar->self()->getRace().getID() == Races::Protoss.getID()) {
		return createProtossAgent(unit);
	}
	if (Broodwar->self()->getRace().getID() == Races::Zerg.getID()) {
		return createZergAgent(unit);
	}

	//Default agents
	if (unit->getType().isWorker()) {
		return new WorkerAgent(unit);
	}
	else if (unit->getType().isBuilding()) {
		return new StructureAgent(unit);
	}
	else {
		return new UnitAgent(unit);
	}
}

BaseAgent* AgentFactory::createTerranAgent(Unit* unit) {
	if (unit->getType().isWorker()) {
		return new WorkerAgent(unit);
	}
	else if (unit->getType().isBuilding()) {
		//Add agents for special buildings here
		if (isOfType(unit, UnitTypes::Terran_Command_Center)) {
			return new CommandCenterAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Barracks)) {
			return new BarracksAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Factory)) {
			return new FactoryAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Engineering_Bay)) {
			return new EngineeringBayAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Academy)) {
			return new AcademyAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Armory)) {
			return new ArmoryAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Machine_Shop)) {
			return new MachineShopAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Starport)) {
			return new StarportAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Science_Facility)) {
			return new ScienceFacilityAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Refinery)) {
			return new RefineryAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Physics_Lab)) {
			return new PhysicsLabAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Control_Tower)) {
			return new ControlTowerAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Covert_Ops)) {
			return new CovertOpsAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Comsat_Station)) {
			return new ComsatAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Bunker)) {
			return new BunkerAgent(unit);
		}
		else {
			//Default structure agent
			return new StructureAgent(unit);
		}
	}
	else {
#if DISABLE_UNIT_AI == 0
		if (isOfType(unit, UnitTypes::Terran_Siege_Tank_Tank_Mode)) {
			return new SiegeTankAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Siege_Tank_Siege_Mode)) {
			return new SiegeTankAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Marine)) {
			return new MarineAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Firebat)) {
			return new FirebatAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Medic)) {
			return new MedicAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Vulture)) {
			return new VultureAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Science_Vessel)) {
			return new ScienceVesselAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Battlecruiser)) {
			return new BattlecruiserAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Wraith)) {
			return new WraithAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Ghost)) {
			return new GhostAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Goliath)) {
			return new GoliathAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Valkyrie)) {
			return new ValkyrieAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Terran_Dropship)) {
			return new DropshipAgent(unit);
		}
		else {
			//Default unit agent
			return new UnitAgent(unit);
		}
#else
		return new UnitAgent(unit);
#endif
	}
	return NULL;
}

BaseAgent* AgentFactory::createProtossAgent(Unit* unit) {
	if (unit->getType().isWorker()) {
		return new WorkerAgent(unit);
	}
	else if (unit->getType().isBuilding()) {
		//Add agents for special buildings here
		if (isOfType(unit, UnitTypes::Protoss_Nexus)) {
			return new NexusAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Protoss_Gateway)) {
			return new GatewayAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Protoss_Assimilator)) {
			return new AssimilatorAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Protoss_Cybernetics_Core)) {
			return new CyberneticsCoreAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Protoss_Robotics_Facility)) {
			return new RoboticsFacilityAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Protoss_Robotics_Support_Bay)) {
			return new RoboticsSupportBayAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Protoss_Observatory)) {
			return new ObservatoryAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Protoss_Citadel_of_Adun)) {
			return new CitadelOfAdunAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Protoss_Templar_Archives)) {
			return new TemplarArchivesAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Protoss_Stargate)) {
			return new StargateAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Protoss_Forge)) {
			return new ForgeAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Protoss_Fleet_Beacon)) {
			return new FleetBeaconAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Protoss_Arbiter_Tribunal)) {
			return new ArbiterTribunalAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Protoss_Shield_Battery)) {
			return new ShieldBatteryAgent(unit);
		}
		else {
			//Default structure agent
			return new StructureAgent(unit);
		}
	}
	else {
#if DISABLE_UNIT_AI == 0
		if (isOfType(unit, UnitTypes::Protoss_Zealot)) {
			return new ZealotAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Protoss_Dragoon)) {
			return new DragoonAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Protoss_Reaver)) {
			return new ReaverAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Protoss_Observer)) {
			return new ObserverAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Protoss_Dark_Templar)) {
			return new DarkTemplarAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Protoss_Scout)) {
			return new ScoutAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Protoss_Corsair)) {
			return new CorsairAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Protoss_High_Templar)) {
			return new HighTemplarAgent(unit);
		}
		else {
			//Default unit agent
			return new UnitAgent(unit);
		}
#else
		return new UnitAgent(unit);
#endif
	}
	return NULL;
}


BaseAgent* AgentFactory::createZergAgent(Unit* unit) {
	Broodwar->printf("Creating zerg agent...");
	Broodwar->printf("is of type? %d", unit->getType());
	//Broodwar->printf("is worker? %s", unit->getType().isWorker());
	if (unit->getType().isWorker()) {
		return new WorkerAgent(unit);
	}
	else if (unit->getType().isBuilding()) {
		//Add agents for special buildings here
		if (isOfType(unit, UnitTypes::Zerg_Hatchery)) {
			return new HatcheryAgent(unit);
		}
		else {
			//Default structure agent
			return new StructureAgent(unit);
		}
	}
	else {
#if DISABLE_UNIT_AI == 0
		if (isOfType(unit, UnitTypes::Zerg_Drone)) {
			// TODO : ...
			return new ZealotAgent(unit);
		}
		else if (isOfType(unit, UnitTypes::Zerg_Zergling)) {
			// TODO : ...
			return new ZealotAgent(unit);
		}
		else {
			//Default unit agent
			return new UnitAgent(unit);
		}
#else
		return new UnitAgent(unit);
#endif
	}
	return NULL;
}

bool AgentFactory::isOfType(Unit* unit, UnitType type) {
	if (unit->getType().getID() == type.getID()) {
		return true;
	}
	return false;
}

