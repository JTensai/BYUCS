#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;


class AccountInterface
{
	public:
		AccountInterface(){}
		virtual ~AccountInterface(){}

		/*
		   getAccountNumber()

		   returns an integer which represents the number of the account

		*/
		virtual int getAccountNumber() = 0;

		/*
			getCurrentBalance()

			returns the balance in the account
		*/

		virtual double getCurrentBalance()= 0;

		/*
			getAccountOwner

			returns the name associated with the account
		*/

		virtual string getAccountOwner()= 0;

		/*
			accountAdvanceMonth()

			Regular Checking:
			A regular checking accounts has a monthly fee of 6 US dollars
			which is deducted  automatically from the balance

			Checking+:
			There is no monthly fee for Checking+ accounts unless a monthly fee is imposed.
			The monthly fee of 6 US dollars is imposed only if the current balance drops below 300 US dollars.

			Checking++:
			There is no monthly fee for Checking++ accounts unless a monthly fee is imposed.
			The montly fee of 6 US dollars is imposed only if the current balance drops below 300 US dollars.
			Checking++ gains monthly interest of 1/12 of .5% (Checking ++ earns .5% per year; therefore
			earns .005/12 per month) only if the current balance is 800 or more. The monthly interest is 
			forfeited if the current balance drops below 800 US dollars.
			 
			Regular Savings:
			Regular Savings has no monthly fee and earns interest of 1% per year. (Regular Savings earns
			.01/12 of interest per month)

			Savings+:
			Earns interest of 1.25% per year. If the balance drops below 1000 US dollars, the interest drops to 1% per year. 

			CD:
			CD's earn interest of 2% per year. If part or all of the CD account is withdrawn from before maturity (9 months), then
			any interest since the withdrawal date is forfeited.

		*/

		virtual void accountAdvanceMonth() = 0;

		/*
			deposit(double amount)

			All accounts allow deposits. The amount of the deposit is added to the current balance.
		*/

		virtual void deposit(double amount) = 0;

		/*

			withdrawFromSavings(double amount)

			If called from any checking account, it must return false.
			
			Regular Savings:
			Withdraw only if the balance remains nonnegative. If it drops below 0 when attempting to withdraw, 
			a fee of 5 US dollars is imposed immediately on the account; return false. 
			Return true otherwise and deduct the amount from the currentBalance.

			Savings+:
			Return true if the (current - amount) is at least 1000 US dollars. Return true, but drop interest
			from .125% to .1% if the balance drops below 1000 dollars. Return false if the (current balance - amount) drops below 0 and
			apply a $5 fee to the current balance. 

			CD:
			Return false if a withdrawal occurs before maturity (9 months), deduct the amount from the current balance, and forfeit
			any interest since the withdrawl date. Return true otherwise and withdraw all of the current balance. If insuffient funds,
			apply a $5 fee to the current balance.
			

		*/

		virtual bool withdrawFromSavings(double amount) = 0;

		/*
			writeCheck(amount)

			If called from any saving/CD account, return false.

			Regular Checking:
			If ( current balance - amount ) is below zero, apply a $5 penalty fee to the current balance and return 
			false. Otherwise subtract the amount from the current balance and return true.

			Checking+:
			If the ( current balance - amount) is at least 300 dollars, subtract the amount from the current balance and return true.
			If the ( current balance - amount) is below  300 dollars, impose a monthly fee of $6, subtract the amount from the current
			balance, and return true.  If there are insuffient funds, apply a $5 fee to the current balance and return
			false.

			Checking++:
			If the ( current balance - amount) is at least 800 dollars, substract the amount from the current balance
			and return true. If the current balance drops below 800, take away the monthly interest,
			subtract the amount from the current balance and return true. If it drops below $300, impose a monthly fee of $6. If there are insuffienct funds, immediately apply a $5 fee 
			to the current balance.

			***MONTHLY fees should ONLY be deducted from an account when the advanceMonth() function is called.*** 
			
		*/

		virtual bool writeCheck(double amount) = 0;



};

