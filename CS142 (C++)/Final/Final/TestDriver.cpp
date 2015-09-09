#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include"Factory.h"
#include"BankInterface.h"
using namespace std;

bool convertStringToBool(string myBool)
{
	if (myBool == "true")
		return true;
	if (myBool == "false")
		return false;
}

bool testAddingCorrectly(BankInterface*& bank, int expectedId ,string expectedName, double expectedBalance)
{
	AccountInterface* account = bank->getAnAccount(expectedId);
	if ( account == NULL)
		return false;
	if ( account->getCurrentBalance() != expectedBalance)
		return false;
	if ( account->getAccountOwner() != expectedName)
		return false;

	return true;

}

bool testOpeningAccounts(ifstream& reader,BankInterface*& bank)
{
	int expectedId = 0;
	string title;
	getline(reader,title);
	bool end = false;
	while( !end)
	{
		string accountType;
		reader >> accountType;
		if ( accountType != "Size:")
		{
			double balance;
			reader >> balance;
			string name;
			reader >> name;
			string expectedResult;
			reader >> expectedResult;

			stringstream info;
			info << accountType << " " << balance << " " << name;
			string infoStr = info.str();
			bool testResult = bank->openAnAccount(infoStr);
			if ( testResult == convertStringToBool(expectedResult))
			{
				bool correctlyAdded = true;
				if (testResult == true)
					correctlyAdded = testAddingCorrectly(bank, ++expectedId ,name, balance);
				if ( correctlyAdded == false)
				{
					cout << "Error: Failed added/rejected account" << endl;
					return false;
				}
				cout << "Successfully added/rejected account" << endl;
			}
			else
			{
				cout << "Error: Failed added/rejected account" << endl;
				system("pause");
				return false;
			}
		}
		else
		{
			end = true;
			int size;
			reader >> size;
			if (bank->getNumberOfAccounts() != size)
			{
				cout << "Error: Incorrect Amount of Accounts Added to Bank" << endl;
				return false;
			}
			cout << "Correct Amount of Accounts Added to Bank" << endl;
		}
	}
	return true;
}

bool testClosingAccounts(ifstream& reader, BankInterface*& bank)
{
	string title;
	getline(reader, title); // may keep or delete (test files are tricky)
	getline(reader, title);
	string action;
	reader >> action;
	
	do
	{
		int id;
		reader >> id;
		string resultStr;
		reader >> resultStr;
		bool expectedResult = convertStringToBool(resultStr);
		bool result = bank->closeAnAccount(id);
		if (expectedResult != result)
		{
			cout<< "Error: Did not close account accordingly" << endl;
			return false;
		}
		cout << "Successfully closed account" << endl;
		reader >> action;

	} 
	while(action != "Size:");
	int size;
	reader >> size;
	if ( size != bank->getNumberOfAccounts())
	{
		cout << "Error: Did not close right number of accounts" << endl;
		return false;
	}
	return true;	
}

bool testWriteCheck(ifstream& reader, BankInterface*& bank, int id)
{
	double param;
	reader >> param;
	string resultStr;
	reader >> resultStr;
	bool expectedResult = convertStringToBool(resultStr);
	AccountInterface* account = bank->getAnAccount(id);
	bool result = account->writeCheck(param);
	if ( result != expectedResult)
	{
		cout << "Error: Did not write check correctly" << endl;
		return false;
	}
	return true;
}

bool testWithdrawFromSavings(ifstream& reader, BankInterface*& bank, int id)
{
	double param;
	reader >> param;
	string resultStr;
	reader >> resultStr;
	bool expectedResult = convertStringToBool(resultStr);
	AccountInterface* account = bank->getAnAccount(id);
	bool result = account->withdrawFromSavings(param);
	//cout << result << endl;
	if ( result != expectedResult)
	{
		cout << "Error: Did not withdraw from savings correctly" << endl;
		return false;
	}
	return true;
}

double roundToNearestWholeNumber(double value)
{
	value = value + .5;
	int roundedValue = value;
	return (double)roundedValue;
}

bool testCurrentBalance(ifstream& reader, BankInterface*& bank, int id)
{
	double expectedResult;
    reader >> expectedResult;
	AccountInterface* account = bank->getAnAccount(id);
	double result = account->getCurrentBalance();
	result = roundToNearestWholeNumber(result);
	cout << "Current Balance: " << result << endl;
	if ( expectedResult != result)
	{
		cout << "Error: Did not have correct current balance" << endl;
		return false;
	}
	return true;
}



bool testingTransactions(ifstream& reader, BankInterface*& bank)
{
	string title;
	getline(reader,title); // may keep or delete (test files are tricky)
	getline(reader,title);
	string method;
	reader >> method;
	do
	{
		
		if ( method == "writeCheck")
		{
			int id;
			reader >> id;
			bool pass = testWriteCheck(reader, bank, id);
			if ( pass == false)
				return false;
		}
		if ( method == "getCurrentBalance")
		{
			int id;
			reader >> id;
			bool pass = testCurrentBalance(reader, bank, id);
			
			if ( pass == false)
				return false;
		}
		if ( method == "advanceMonth")
		{
			bank->advanceMonth();
			//cout << "Month has passed\n";
		}
		if ( method == "deposit")
		{
			int id;
			reader >> id;
			double depositAmount;
			reader >> depositAmount;
			AccountInterface* account = bank->getAnAccount(id);
			account->deposit(depositAmount);
		}
		if (method == "withdrawFromSavings")
		{
			int id;
			reader >> id;
			bool pass = testWithdrawFromSavings(reader, bank, id);
			if ( pass == false)
				return false;
		}

		cout << "Successfully completed Transaction"<<endl;
		method = ""; 
		reader >> method;
	}while( method != "End");

	return true;
}





int main()
{
	// creates bank pointer
	Factory* produce = new Factory();
	BankInterface* bank = produce->createBank();
	// opening test file
	ifstream reader;
	string filename;
	cout << "Enter the test file with the extension (.txt): " ;
	cin >> filename;
	reader.open(filename.c_str());
	

	// testing adding
	cout << "------------------------------------------" << endl;
	cout << "Testing Opening Accounts" << endl;
	bool passedOpening = testOpeningAccounts(reader, bank);
	if ( passedOpening == false)
	{
		system("pause");
		return 0;
	}
	cout << "Opening Accounts Tests Passed" << endl;

	//testing transactions
	cout << "------------------------------------------" << endl;
	cout << "Testing Account Transactions" << endl;
	bool passedChecking = testingTransactions(reader, bank);
	if ( passedChecking == false)
	{
		system("pause");
		return 0;
	}
	cout << "Account Transaction Tests Passed" << endl;

	//testing closing accounts
	cout << "------------------------------------------" << endl;
	bool passedClosing = testClosingAccounts(reader, bank);
	if ( passedClosing == false)
	{
		system("pause");
		return 0;
	}
	cout << "Closing Accounts Tests Passed" << endl;

	// closing test file
	reader.close();




	system("pause");
	return 0;
}