#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Account.h"

using namespace std;

class Checking_pp: public Account
{
	//private:
		//int maxMana, currMana;

	public:
		Checking_pp(string type, double balance, string name, int accountNumber);
		virtual ~Checking_pp(){}
		
		virtual void accountAdvanceMonth();
};
