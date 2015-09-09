#pragma once
#include "Cleric.h"

using namespace std;


Cleric::Cleric(string name, int mhp, int strength, int speed, int magic)
	:Fighter(name, mhp, strength, speed, magic)
{
	maxMana = 5*magic;
	currMana = maxMana;
}

int Cleric::getDamage()
{
	return magic;
	//Cleric:
	//This value is equal to the Cleric's magic.
}

void Cleric::reset()
{
	Fighter::reset();
	currMana = maxMana;

	//Cleric:
	//Also restores a Cleric's current mana to its maximum value (which is 5 times its magic).
}

void Cleric::regenerate()
{
	Fighter::regenerate();
	int increase = magic/5;

	if (increase == 0){
		increase = 1;
	}
	currMana = currMana + increase;

	if (currMana > maxMana){
		currMana = maxMana;
	}
	//Cleric:
	//Also increases a Cleric's current mana by an amount equal to one fifth of the 
	//Cleric's magic.  This method must increase the Cleric's current mana by at 
	//least one.  Do not allow the current mana to exceed the maximum mana.
}

bool Cleric::useAbility()
{
	if (currMana >= CLERIC_ABILITY_COST)
	{
		int increase = magic/3;
		if (increase == 0){
			increase = 1;
		}
		chp = chp + increase;
		if (chp > mhp){
			chp = mhp;
		}
		currMana = currMana - CLERIC_ABILITY_COST;
		
		return true;
	}
	else return false;
	/*	Cleric: Healing Light
	*	Increases the Cleric's current hit points by an amount equal to one third of its magic.
	*	Can only be used if the Cleric has at least [CLERIC_ABILITY_COST] mana.
	*	Will be used even if the Cleric's current HP is equal to their maximum HP.
	*	Decreases the Cleric's current mana by [CLERIC_ABILITY_COST] when used.
	*	Cleric Note:
	*	This ability, when successful, must increase the Cleric's current hit points 
	*	by at least one.  Do not allow the current hit points to exceed the maximum 
	*	hit points.
	*/
}
