#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Account.h"

using namespace std;

class Checking: public Account
{
	//private:
		//int maxMana, currMana;

	public:
		Checking(string type, double balance, string name, int accountNumber);
		virtual ~Checking(){}

		virtual void accountAdvanceMonth();

};
