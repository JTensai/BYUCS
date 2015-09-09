#pragma once
#include "Checking_pp.h"

using namespace std;


Checking_pp::Checking_pp(string type, double balance, string name, int accountNumber)
	:Account(type, balance, name, accountNumber)
{
}

void Checking_pp::accountAdvanceMonth(){
	if (balance >= 800){
		double interest;
		interest = balance * (.005/12); //.5% interest per anum
		balance = balance + interest;
	}
	else if (balance < 300) {
		balance = balance - 6; //Monthly fee for Checking
	}	
}