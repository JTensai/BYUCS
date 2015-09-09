#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Fighter.h"

using namespace std;

class Robot: public Fighter
{
	private:
		int currEnergy, maxEnergy, bonusDamage;

	public:
		Robot(string name, int mhp, int strength, int speed, int magic);
		virtual ~Robot(){}
		
		virtual int getDamage();

		virtual void reset();

		virtual void regenerate();

		virtual bool useAbility();
};
