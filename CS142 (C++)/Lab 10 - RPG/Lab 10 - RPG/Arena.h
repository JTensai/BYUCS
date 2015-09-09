#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "ArenaInterface.h"
//#include "Fighter.h"
//#include "FighterInterface.h"
using namespace std;

class Arena: public ArenaInterface
{
	private:
		vector <FighterInterface*> allFighters;

	public:
		Arena();
		virtual ~Arena();

		virtual bool addFighter(string info);

		virtual bool removeFighter(string name);
		
		virtual FighterInterface* getFighter(string name);
		
		virtual int getSize();
};
