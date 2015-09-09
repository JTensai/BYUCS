#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Account.h"

using namespace std;

class Saving: public Account
{
	//private:
		//int maxMana, currMana;

	public:
		Saving(string type, double balance, string name, int accountNumber);
		virtual ~Saving(){}
			
		virtual void accountAdvanceMonth();

		virtual bool withdrawFromSavings(double amount);
		
};
