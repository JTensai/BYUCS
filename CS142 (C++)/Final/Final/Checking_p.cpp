#pragma once
#include "Checking_p.h"

using namespace std;


Checking_p::Checking_p(string type, double balance, string name, int accountNumber)
	:Account(type, balance, name, accountNumber)
{
}

void Checking_p::accountAdvanceMonth(){
	if (balance < 300){
		balance = balance - 6; //Monthly fee for Checking
	}
}