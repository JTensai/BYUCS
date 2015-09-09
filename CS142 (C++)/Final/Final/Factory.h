#pragma once
#include "BankInterface.h"
#include "Bank.h"

using namespace std;

class Factory
{
	public:
		Factory(void);

		BankInterface* createBank();

		~Factory(void);
};

