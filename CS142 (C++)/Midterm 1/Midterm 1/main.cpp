#include <iostream>
#include <string>

using namespace std;

/******************** Test Cases ********************

*********Test 1:**********
Testing rejection of invalid input:

Choose Option: 1
Enter amount of US Dollars you would like to exchange: -6

Error: Invalid input, returning to main menu


Choose Option: 2
Enter amount of US Dollars you would like to exchange: 0

Error: Invalid input, returning to main menu


Choose Option: 4
Enter amount of US Dollars you would like to exchange: 4

What coin would you like the most of? jordan

Error: Invalid input, returning to main menu


*********Test 2:**********
Choose Option: 1
Enter amount of US Dollars you would like to exchange: 263

Your change for 263 Dollars:

Burton:   5		(5*44) = 220
Seppi:    1		(1*29) = 29
Clement:  1		(1*8)  = 8
Child:    2		(2*3)  = 6
Tee-A:    0		(0*1)  = 0

220 + 29 + 8 + 6 + 0 = 263 (Correct)

********Test 3:**********

Choose Option: 2
Enter amount of US Dollars you would like to exchange: 7

Your change for 7 Dollars:

Burton:   0
Seppi:    0
Clement:  0
Child:    2
Tee-A:    1

6+1 = 7

Your change for 6 Dollars:

Burton:   0
Seppi:    0
Clement:  0
Child:    2
Tee-A:    0

6 = 6

Your change for 5 Dollars:

Burton:   0
Seppi:    0
Clement:  0
Child:    1
Tee-A:    2

3+2 = 5

Your change for 4 Dollars:

Burton:   0
Seppi:    0
Clement:  0
Child:    1
Tee-A:    1

3+1 = 4

Your change for 3 Dollars:

Burton:   0
Seppi:    0
Clement:  0
Child:    1
Tee-A:    0

3 = 3

Your change for 2 Dollars:

Burton:   0
Seppi:    0
Clement:  0
Child:    0
Tee-A:    2

2 = 2

Your change for 1 Dollar:

Burton:   0
Seppi:    0
Clement:  0
Child:    0
Tee-A:    1

1 = 1

All are correct.

********Test 4: (Extra Credit)************

Choose Option: 4
Enter amount of US Dollars you would like to exchange:
163

What coin would you like the most of? clement
Your change for 163 Dollars:

Burton:   0
Seppi:    0
Clement:  20	(20*8)= 160
Child:    1		(1*3) = 3
Tee-A:    0

160 + 3 = 163    Correct


****************************************************/

/******************** Variables ********************/

double menu_option;
int val_burton = 44, val_seppi = 29, val_clement = 8, val_child = 3, val_ta = 1, starting_money; 
string coin_type_name = "none";
string coin_types[] = {"burton", "seppi", "clement", "child", "tee-a"};
bool repeat = true;

/***************************************************/

// Load the main menu and prompts for selection, reprompting until a valid input is given
int main_menu(){
	cout << "\n\tMain Menu" << endl << "1. Exchange US Dollars to C-MASMAS Coins" << endl << "2. Show a list of equivalents" << endl << "3. Quit" << endl << "4. Exchange US Dollars to C-MASMAS coins in mostly one coin" << endl << "\nChoose Option: ";
	cin >> menu_option;
	
	while ( menu_option > 4 || menu_option < 1 ){
		cin.clear();
		cout << "\nPlease select valid option (1-4): ";
		cin >> menu_option;
	}
	return menu_option;
}

// validates that the input for starting dollars is valid
bool money_input(){
	cout << "Enter amount of US Dollars you would like to exchange: ";
	cin >> starting_money;
	cout << endl;
	if (starting_money < 1){
		cin.clear();
		cout << "Error: Invalid input, returning to main menu" << endl;
		return false;
	}
	else{
		return true;
	}
}

// Validates that the input for most wanted coin is correct
bool coin_type(){
	bool valid_string = false;
	cout << "What coin would you like the most of? ";
	cin.ignore();
	getline(cin, coin_type_name);
	for (int i=0; i<5; i++){
		if (coin_type_name == coin_types[i])
			valid_string = true;
	}
	if (valid_string == false || coin_type_name.empty()){
		cout << "\nError: Invalid input, returning to main menu\n" << endl;
		return false;
	}
	else{
		return true;
	}
}

// Output for how many coins should be given as change
int coin_output(int burtons, int seppis, int clements, int childs, int tas, int dollars){
	
	if (dollars > 1){
		cout << "Your change for " << dollars << " Dollars:" << endl;
	}
	else{
		cout << "Your change for " << dollars << " Dollar:" << endl;
	}

	cout << "\nBurton:   " << burtons << endl
		<< "Seppi:    " << seppis << endl
		<< "Clement:  " << clements << endl
		<< "Child:    " << childs << endl
		<< "Tee-A:    " << tas << "\n" << endl;
	return 0;
}


// Uses multiple inputs to determine how many of each coin is required based on the given parameters
int exchange(int input_money){
	int num_burtons = 0, num_seppis = 0, num_clements = 0, num_childs = 0, num_tas = 0;
	int current_money = input_money;

	// Default setting for which coin is most wanted
	if (coin_type_name == "none" || coin_type_name == "burton"){
		while(current_money >= val_burton){
			current_money = current_money - val_burton;
			num_burtons++;
		}
		while(current_money >= val_seppi){
			current_money = current_money - val_seppi;
			num_seppis++;
		}
		while(current_money >= val_clement){
			current_money = current_money - val_clement;
			num_clements++;
		}
		while(current_money >= val_child){
			current_money = current_money - val_child;
			num_childs++;
		}
		while(current_money >= val_ta){
			current_money = current_money - val_ta;
			num_tas++;
		}
	}
	
	// used when the most wanted coin in Seppi
	else if (coin_type_name == "seppi"){
		while(current_money >= val_seppi){
			current_money = current_money - val_seppi;
			num_seppis++;
		}
		while(current_money >= val_clement){
			current_money = current_money - val_clement;
			num_clements++;
		}
		while(current_money >= val_child){
			current_money = current_money - val_child;
			num_childs++;
		}
		while(current_money >= val_ta){
			current_money = current_money - val_ta;
			num_tas++;
		}
	}

	// used when the most wanted coin in Clement
	else if (coin_type_name == "clement"){
		while(current_money >= val_clement){
			current_money = current_money - val_clement;
			num_clements++;
		}
		while(current_money >= val_child){
			current_money = current_money - val_child;
			num_childs++;
		}
		while(current_money >= val_ta){
			current_money = current_money - val_ta;
			num_tas++;
		}
	}
	
	// used when the most wanted coin in Child
	else if (coin_type_name == "child"){
		while(current_money >= val_child){
			current_money = current_money - val_child;
			num_childs++;
		}
		while(current_money >= val_ta){
			current_money = current_money - val_ta;
			num_tas++;
		}
	}
	
	// used when the most wanted coin in Tee-A
	else if (coin_type_name == "tee-a"){
		while(current_money >= val_ta){
			current_money = current_money - val_ta;
			num_tas++;
		}
	}

	// calls the coin_output function which handles outputing the number of each coin given as change
	coin_output(num_burtons, num_seppis, num_clements, num_childs, num_tas, input_money);	
	return 0;
}

//Runs the exchange function for the current amount of dollars, then decreases current dollars by one and repeats until current dollars is 0
int decrement(int current_dollars){
	while (current_dollars > 0){
		exchange(current_dollars);
		current_dollars--;
	}
	return 0;
}

int main(){
	while (repeat == true){
		system("CLS");
		// Clears the input for option 4 after each time the program runs
		coin_type_name = "none";
		// Loads main menu
		main_menu();
		if (menu_option == 1){
			// Makes sure input is valid, and if it is, it calls the exchange function passing in the value of starting_money that was set in the money_input function
			if (money_input() == true){
				exchange(starting_money);
			}
			system("pause");
			repeat = true;
		}
		else if (menu_option == 2){
			// Makes sure input is valid, and if it is, it calls the decrement function passing in the value of starting_money that was set in the money_input function
			if (money_input() == true){
				decrement(starting_money);
			}
			system("pause");
			repeat = true;
		}
		else if (menu_option == 3){
			// Quits
			repeat = false;
		}
		else if (menu_option == 4){
			// Makes sure input is valid both for the dollar value and the string input for coin name
			// and if it is, it calls the exchange function passing in the value of starting_money that was set in the money_input function
			if (money_input() == true){
				if (coin_type() == true){
					exchange(starting_money);
				}
			}
			system("pause");
			repeat = true;
		}
	}
	return 0;
}
