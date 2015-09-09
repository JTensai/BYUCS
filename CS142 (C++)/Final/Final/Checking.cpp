#pragma once
#include "Checking.h"

using namespace std;


Checking::Checking(string type, double balance, string name, int accountNumber)
	:Account(type, balance, name, accountNumber)
{
}

void Checking::accountAdvanceMonth(){
	balance = balance - 6; //Monthly fee for Checking
}
