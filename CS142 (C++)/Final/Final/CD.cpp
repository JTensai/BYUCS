#pragma once
#include "CD.h"

using namespace std;


CD::CD(string type, double balance, string name, int accountNumber)
	:Account(type, balance, name, accountNumber)
{
	month = 0;
	init_balance = balance;
	gain = true;
}

void CD::accountAdvanceMonth(){
	if (gain == true){
		month++; 
		double interest;
		interest = balance * (.02/12); //1% interest per anum
		balance = balance + interest;
	}
}


bool CD::withdrawFromSavings(double amount){
	if (month < 9){	
		if (amount <= balance){
			gain = false;
			balance = init_balance;
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
	else{
		if (amount <= balance){
			balance = 0;
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

}
	