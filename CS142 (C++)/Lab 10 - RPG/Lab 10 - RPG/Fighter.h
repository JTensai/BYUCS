#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "FighterInterface.h"
using namespace std;

class Fighter: public FighterInterface
{
	protected:
		string name;
		int mhp;
		int chp;
		int strength;
		int speed;
		int magic;

	public:
		Fighter(string name, int mhp, int strength, int speed, int magic);
		virtual ~Fighter(){}
		
		string getName();

		int getMaximumHP();

		int getCurrentHP();
		
		int getStrength();
		
		int getSpeed();
		
		int getMagic();
		
		virtual int getDamage() = 0;
		
		void takeDamage(int damage);
		
		virtual void reset();
		
		virtual void regenerate();
		
		virtual bool useAbility() = 0;
};
