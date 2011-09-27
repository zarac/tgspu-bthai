#include "PFFunctions.h"
#include "MedicAgent.h"

float PFFunctions::getDistance(Position p1, Position p2)
{
	return (float)p1.getDistance(p2);
}

float PFFunctions::getDistance(Position pos, Unit* unit)
{
	return (float)unit->getDistance(pos);
}

int PFFunctions::getSize(UnitType type) {
	if (type.isWorker()) {
		return 6;
	}
	if (type.size() == UnitSizeTypes::Small) {
		return 6;
	}
	if (type.size() == UnitSizeTypes::Medium) {
		return 12;
	}
	if (type.size() == UnitSizeTypes::Large) {
		return 16;
	}
	return 12;
}

float PFFunctions::calcOwnUnitP(float d, Unit* unit, Unit* otherOwnUnit)
{
	if (unit->getID() == otherOwnUnit->getID()) {
		//Dont count collision with yourself...
		return 0;
	}

	float p = 0;

	if (BaseAgent::isOfType(otherOwnUnit, UnitTypes::Terran_Vulture_Spider_Mine)) {
		//Make sure to avoid mines
		if (d <= 125 + getSize(unit->getType())) {
			p = -20;
		}
	}

	if (unit->isCloaked() && !otherOwnUnit->isCloaked()) {
		//Let cloaked units stay away from non-cloaked units to avoid
		//getting killed by splash damage.
		if (d <= 50 + getSize(unit->getType())) {
			p = -20;
		}
	}

    if (d <= getSize(unit->getType())) {
        p = -20;
    }

    return p;
}

float PFFunctions::calcMineP(float d, Unit* unit) {
	float p = 0;

	if (unit->getType().isFlyer()) {
		p = 0;
	}
	else if (unit->getType().isFlyingBuilding()) {
		p = 0;
	}
	else {
		if (d <= getSize(unit->getType())) {
			p = -40;
		}
	}

	return p;
}

float PFFunctions::calcOwnUnitMedicP(float d, Unit* unit, Unit* otherOwnUnit)
{
	if (unit->getID() == otherOwnUnit->getID()) {
		//Dont count collision with yourself...
		return 0;
	}

    float p = 0;

	if (MedicAgent::isMedicTarget(otherOwnUnit)) { //Stay close to organic attacking units
		//This unit can be healed
		if (d <= getSize(unit->getType())) {
			//Avoid collision
			p = -20;
		}
		else if (d <= 32) {
			//Dont stand too close, if we can avoid it
			p = 0;
		}
		else {
			//Move towards the unit
			p = calcNavigationP(d);
		}
	}
	else {
		if (d <= getSize(unit->getType())) {
			//Avoid collision
			p = -20;
		}
	}

    return p;
}

float PFFunctions::calcNavigationP(float d) {
    float p = (float)200 - (float)0.1 * d;
    if (p < 0)
    {
        p = 0;
    }
    return p;
}

float PFFunctions::getNavigationP(Position cPos, Position goal) {
    float dist = getDistance(cPos, goal);
    return calcNavigationP(dist);
}

float PFFunctions::calcAttackingUnitP(float d, Unit* attacker, Unit* enemy, bool defensive)
{
	//Check if enemy unit exists and is visible.
	if (!enemy->exists()) {
		return 0;
	}
	if (!enemy->isVisible()) {
		return 0;
	}

    int myMSD = attacker->getType().groundWeapon().maxRange();
	int enemyMSD = enemy->getType().groundWeapon().maxRange();
    
    float p = 0;

	if (!defensive || !canBeAttacked(attacker, enemy) && canBeAttacked(enemy, attacker)) {
		//Offensive mode -> attack
		if (d < myMSD - 20) {
			float fact = (float)200 / myMSD;
			p = d * fact;
			if (p < 0) {
				p = 0;
			}
		}
		else if (d > myMSD - 20 && d < myMSD) {
			p = (float)200;
		}
		else {
			float p = (float)180 - (float)0.1 * d; //0.2 = 2x siege tank range
			if (p < 0)
			{
				p = 0;
			}
		}
	}
	else {
		//Defensive mode -> retreat
		p = (float)(d * 2);
		if (d > enemyMSD * 1.5) {
			p = 0;
		}
	}

    return p;
}

bool PFFunctions::canBeAttacked(Unit* ownUnit, Unit* enemyUnit) {
	UnitType oType = ownUnit->getType();
	UnitType eType = ownUnit->getType();

	if (oType.isFlyer()) {
		//Own unit is air
		if (eType.groundWeapon().targetsAir()) {
			return true;
		}
		if (eType.airWeapon().targetsAir()) {
			return true;
		}
	}
	else {
		//Own unit is ground
		if (eType.groundWeapon().targetsGround()) {
			return true;
		}
		if (eType.airWeapon().targetsGround()) {
			return true;
		}
	}

	return false;
}

float PFFunctions::calcDefensiveUnitP(float d, Unit* ownUnit, Unit* enemy)
{
	int range = 0;
	if (enemy->getType().canAttack()) {
		range = (int)(enemy->getType().groundWeapon().maxRange() * 1.5);
	}
	if (!canBeAttacked(ownUnit, enemy)) {
		range = 0;
	}
    
    float p = (float)(d * 2);
	if (d > range) {
		p = 0;
	}

    return p;
}

