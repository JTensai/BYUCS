#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Account.h"

using namespace std;

class CD: public Account
{
	private:
		double init_balance;
		int month;
		bool gain;

	public:
		CD(string type, double balance, string name, int accountNumber);
		virtual ~CD(){}

		virtual void accountAdvanceMonth();

		virtual bool withdrawFromSavings(double amount);
};
