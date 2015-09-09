#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Bank.h"
#include "Checking.h"
#include "Checking_p.h"
#include "Checking_pp.h"
#include "Saving.h"
#include "Saving_p.h"
#include "CD.h"

using namespace std;

Bank::Bank(){}
Bank::~Bank(){}		


int newAccountNum = 1;

bool Bank::openAnAccount(string info){
	stringstream ss;
	string type, name;
	double balance;
	int accountNumber;

	ss.str(info);

	ss >> type;
	ss >> balance;
	ss >> name;

	if (ss.fail() == true || ss.eof() == false){
		return false;
	}

	AccountInterface* account;
	accountNumber = newAccountNum;

	if (type == "Checking"){
		account = new Checking(type, balance, name, accountNumber);
	}
	else if (type == "Checking+" && balance >= 300){
		account = new Checking_p(type, balance, name, accountNumber);
	}
	else if (type == "Checking++" && balance >= 800){
		account = new Checking_pp(type, balance, name, accountNumber);
	}
	else if (type == "Saving"){
		account = new Saving(type, balance, name, accountNumber);
	}
	else if (type == "Saving+" && balance >= 1000){
		account = new Saving_p(type, balance, name, accountNumber);
	}
	else if (type == "CD"){
		account = new CD(type, balance, name, accountNumber);
	}
	else{
		// invalid account type, return false
		return false;
	}

	if (balance <= 0){
		// invalid account balance, return false
		return false;
	}

	newAccountNum++;
	allAccounts.push_back(account);

	return true;
}

bool Bank::closeAnAccount(int accountNumber){
	for (int i = 0; i < allAccounts.size(); i++){
		if (allAccounts[i]->getAccountNumber() == accountNumber){
			//account exists, check for positive balance
			if (allAccounts[i]->getCurrentBalance() >= 0){
				//remove from allAccounts vector and return true
				allAccounts.erase(allAccounts.begin()+i);
				return true;
			}
			//Negative Balance, cannot close account
		//account does not exists, do nothing
		}
	}	
	return false;
}

AccountInterface* Bank::getAnAccount(int accountNumber){
	for (int i = 0; i < allAccounts.size(); i++){
		if (allAccounts[i]->getAccountNumber() == accountNumber){
			return allAccounts[i];
		}
		else{
			//account does not exists, do nothing
		}
	}
	return NULL;
}

void Bank::advanceMonth(){
	//loop through each account and call advance month
	
	for (int i = 0; i < allAccounts.size(); i++){
		allAccounts[i]->accountAdvanceMonth();
	}
}

int Bank::getNumberOfAccounts(){
	return allAccounts.size();
}