#pragma once
#include "Robot.h"

using namespace std;


Robot::Robot(string name, int mhp, int strength, int speed, int magic)
	:Fighter(name, mhp, strength, speed, magic)
{
	maxEnergy = magic*2;
	currEnergy = maxEnergy;
	bonusDamage = 0;
}


int Robot::getDamage()
{
	int damage = strength + bonusDamage;
	bonusDamage = 0;
	return damage;
	//Robot:
	//This value is equal to the Robot's strength plus any additional damage added for having just used its special ability.
}

void Robot::reset()
{
	Fighter::reset();
	currEnergy = maxEnergy;
	bonusDamage = 0;

	//Robot:
	//Also restores a Robot's current energy to its maximum value (which is 2 times its magic).
	//Also resets a Robot's bonus damage to 0.	
}

void Robot::regenerate()
{	
	Fighter::regenerate();
}

bool Robot::useAbility()
{
	if (currEnergy >= ROBOT_ABILITY_COST){

		double d_strength = strength, current_energy = currEnergy, maximum_energy = maxEnergy, bonus;

		double e = current_energy/maximum_energy;

		bonus = d_strength * (e*e*e*e);

		bonusDamage = bonus;
		
		currEnergy = currEnergy - ROBOT_ABILITY_COST;

		return true;
	}
	else return false;

	/*Robot: Shockwave Punch
	*	Adds bonus damage to the Robot's next attack (and only its next attack) equal to (strength  * ((current_energy/maximum_energy)^4)).
	*	Can only be used if the Robot has at least [ROBOT_ABILITY_COST] energy.
	*	Decreases the Robot's current energy by [ROBOT_ABILITY_COST] (after calculating the additional damage) when used.
	*		Examples:
	*		strength=20, current_energy=20, maximum_energy=20		=> bonus_damage=20
	*		strength=20, current_energy=15, maximum_energy=20		=> bonus_damage=6
	*		strength=20, current_energy=10, maximum_energy=20		=> bonus_damage=1
	*		strength=20, current_energy=5,  maximum_energy=20		=> bonus_damage=0
	*	Robot Note:
	*	The bonus damage formula should be computed using double arithmetic, and only 
	*	the final result should be cast into an integer.
	*/
}
