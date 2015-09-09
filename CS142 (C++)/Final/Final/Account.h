#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "AccountInterface.h"
using namespace std;

class Account: public AccountInterface
{

	protected:
		string type;
		int balance;
		string name;
		int accountNumber;

	Account(string type, double balance, string name, int accountNumber);



	public:
		Account();
		virtual ~Account(){}

		virtual int getAccountNumber();

		virtual double getCurrentBalance();

		virtual string getAccountOwner();


		virtual void deposit(double amount);

		virtual bool withdrawFromSavings(double amount);
		
		virtual bool writeCheck(double amount);

};

