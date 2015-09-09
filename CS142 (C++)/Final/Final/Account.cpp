#pragma once
#include "Account.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

Account::Account(string type, double balance, string name, int accountNumber)
{
	this->type = type;
	this->balance = balance;
	this->name = name;
	this->accountNumber = accountNumber;
}		

//-------------------------------------------------------------------------	

int Account::getAccountNumber(){
	return accountNumber;
}

double Account::getCurrentBalance(){
	return balance;
}

string Account::getAccountOwner(){
	return name;
}

void Account::deposit(double amount){
	balance = balance + amount;
}

bool Account::withdrawFromSavings(double amount){
	return true;
}
		
bool Account::writeCheck(double amount){
	
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

