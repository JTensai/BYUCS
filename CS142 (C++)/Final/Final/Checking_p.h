#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Account.h"

using namespace std;

class Checking_p: public Account
{
	//private:
		//int maxMana, currMana;

	public:
		Checking_p(string type, double balance, string name, int accountNumber);
		virtual ~Checking_p(){}
	
		virtual void accountAdvanceMonth();		
		
};
