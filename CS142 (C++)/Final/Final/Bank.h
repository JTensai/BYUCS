#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "BankInterface.h"

using namespace std;

class Bank: public BankInterface
{	
	private:
		vector <AccountInterface*> allAccounts;

	public:
		Bank();
		virtual ~Bank();

		virtual bool openAnAccount(string info);

		virtual bool closeAnAccount(int accountNumber);

		virtual AccountInterface* getAnAccount(int accountNumber);

		virtual void advanceMonth();

		virtual int getNumberOfAccounts();

		/*
		*****************************************************************************************
		Extra Credit - uncomment this section to add the extra credit function into your program
		*****************************************************************************************

		Implement “getOrderedAccounts(string criterion)” in the BankInterface. 
		The method must return a vector<AccountInterface*> sorted by a criterion. The criterion will 
		be a string that is “accountNumber”, “name”, or “balance”. You must sort the accounts by that 
		criterion. If the criterion is “accountNumber”, the vector must be sorted by increasing account 
		numbers ( 1,2,3…). If it is “name”, it must be sorted by increasing ASCII value ( A,B,C,a,b,c…).
		If the criteria is “balance”, the accounts must be sorted by nondecreasing balance value 
		( 100,230,1200,...).


		virtual vector<AccountInterface*> getSortedAccounts(string criteria) = 0;



		*/
};