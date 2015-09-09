#pragma once
#include "Fighter.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;


/*
 *	This class specifies the methods for a fighter.
 *
 *	All fighters have the following attributes:
 *		Name - The fighter's name
 *		Hit Points - The amount of health the fighter has, with a specified maximum; reaching 0 is equivalent to losing
 *		Strength - Physical power, used to determine hit point regeneration
 *		Speed - Dexterity and physical movement, used to reduce damage when being attacked
 *		Magic - Magical prowess, used for some special abilities
 *
 *	The three fighter types have unique abilities:
 *		Robot - Relies on strength to deal damage.  Also can use stored electricity to temporarily increase damage (max electricity equal to 2*magic).
 *		Archer - Relies on speed to deal damage.  Also can increase its speed for the remainder of the battle (no max bonus speed).
 *		Cleric - Relies on magic to deal damage.  Also can heal itself using mana, restoring hit points (max mana equal to 5*magic).
 *
 *	More details about how stats are used and how abilities work can be found in the comments below.
 */


Fighter::Fighter(string name, int mhp, int strength, int speed, int magic)
{
	this->name = name;
	this->mhp = mhp;
	this->chp = mhp;
	this->strength = strength;
	this->speed = speed;
	this->magic = magic;
}		

//-------------------------------------------------------------------------	

string Fighter::getName()
{
	//getName()
	//Returns the name of this fighter.
	return name;
}

int Fighter::getMaximumHP()
{
	//getMaximumHP()
	//Returns the maximum hit points of this fighter.
	return mhp;
}

int Fighter::getCurrentHP()
{
	//getCurrentHP()
	//Returns the current hit points of this fighter.
	return chp;
}

int Fighter::getStrength()
{
	//getStrength()
	//Returns the strength stat of this fighter.
	return strength;
}

int Fighter::getSpeed()
{
	//getSpeed()
	//Returns the speed stat of this fighter.
	return speed;
}

int Fighter::getMagic()
{
	//getMagic()
	//Returns the magic stat of this fighter.
	return magic;
}

void Fighter::takeDamage(int damage)
{
	int damage_done = damage - (speed/4);
	if (damage_done < 1){
		damage_done = 1;
	}
	chp = chp - damage_done;
	

	//takeDamage(int)
	
	//	Reduces the fighter's current hit points by an amount equal to the given 
	//	damage minus one fourth of the fighter's speed.  This method must reduce 
	//	the fighter's current hit points by at least one.  It is acceptable for 
	//	this method to give the fighter negative current hit points.
	
	//Examples:
	//	damage=10, speed=7		=> damage_taken=9
	//	damage=10, speed=9		=> damage_taken=8
	//	damage=10, speed=50		=> damage_taken=1
}

void Fighter::reset()
{
	chp = mhp;

	//reset()
	//Restores a fighter's current hit points to its maximum hit points.	
}

void Fighter::regenerate()
{
	int increase = strength/6;
	if (increase == 0){
		increase = 1;
	}
	chp = chp + increase;
	if (chp > mhp){
		chp = mhp;
	}

	//regenerate()	
	//Increases the fighter's current hit points by an amount equal to one sixth of
	//the fighter's strength.  This method must increase the fighter's current hit 
	//points by at least one.  Do not allow the current hit points to exceed the 
	//maximum hit points.	
}

//------------------------------------------------------------------------------------------