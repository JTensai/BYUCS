#pragma once
#include "Saving_p.h"

using namespace std;


Saving_p::Saving_p(string type, double balance, string name, int accountNumber)
	:Account(type, balance, name, accountNumber)
{
}

void Saving_p::accountAdvanceMonth(){
	double interest;
	if (balance >= 1000){
		interest = balance * (.0125/12); //1.25% interest per anum
	}
	else {
		interest = balance * (.01/12); //1% interest per anum
	}
	balance = balance + interest;
}

bool Saving_p::withdrawFromSavings(double amount){
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