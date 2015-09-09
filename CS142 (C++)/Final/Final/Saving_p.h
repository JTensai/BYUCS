#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Account.h"

using namespace std;

class Saving_p: public Account
{
	//private:
		//int maxMana, currMana;

	public:
		Saving_p(string type, double balance, string name, int accountNumber);
		virtual ~Saving_p(){}

		virtual void accountAdvanceMonth();
		
		virtual bool withdrawFromSavings(double amount);
};
