#pragma once
#include "Archer.h"

using namespace std;


Archer::Archer(string name, int mhp, int strength, int speed, int magic)
	:Fighter(name, mhp, strength, speed, magic)
{
	dyn_speed = speed;
}

int Archer::getDamage()
{
	//return dyn_speed;
	return speed;
	//Archer:
	//This value is equal to the Archer's speed.
}

void Archer::reset()
{
	Fighter::reset();
	//dyn_speed = speed;
	speed = dyn_speed;
	//Archer:
	//Also resets an Archer's current speed to its original value.
}

void Archer::regenerate()
{
	Fighter::regenerate();
}

bool Archer::useAbility()
{
	//dyn_speed++;
	speed++;

	/*	Archer: Quickstep
	*	Increases the Archer's speed by one point each time the ability is used.
	*	This bonus lasts until the reset() method is used.
	*	This ability always works; there is no maximum bonus speed.
	*/
	return true;
}
