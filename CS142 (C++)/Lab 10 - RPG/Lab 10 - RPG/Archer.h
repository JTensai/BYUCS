#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Fighter.h"

using namespace std;

class Archer: public Fighter
{
	private:
		int dyn_speed;

	public:
		Archer(string name, int mhp, int strength, int speed, int magic);
		virtual ~Archer(){}
				
		virtual int getDamage();

		virtual void reset();

		virtual void regenerate();

		virtual bool useAbility();
};
