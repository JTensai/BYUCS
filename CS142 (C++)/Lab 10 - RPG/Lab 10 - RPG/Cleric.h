#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Fighter.h"

using namespace std;

class Cleric: public Fighter
{
	private:
		int maxMana, currMana;

	public:
		Cleric(string name, int mhp, int strength, int speed, int magic);
		virtual ~Cleric(){}
				
		virtual int getDamage();

		virtual void reset();

		virtual void regenerate();

		virtual bool useAbility();
};
