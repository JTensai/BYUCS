#pragma once
#include "Saving.h"

using namespace std;


Saving::Saving(string type, double balance, string name, int accountNumber)
	:Account(type, balance, name, accountNumber)
{
}

void Saving::accountAdvanceMonth(){
	double interest;
	interest = balance * (.01/12); //1% interest per anum
	balance = balance + interest;
}

bool Saving::withdrawFromSavings(double amount){
	if (amount <= balance){
		balance = balance - amount;
		return true;
	}
	else{
		if (balance <= 5){
			balance = 0;
		}
		else{
			balance = balance - 5;
		}
		return false;
	}
}
		