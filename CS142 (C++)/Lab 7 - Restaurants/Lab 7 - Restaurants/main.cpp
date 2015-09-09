#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h> 

using namespace std;

/********************* Variables ********************
Test 1: Chose add Restaurant. Typed in "Taco Bell". Chose to display all Restaurants. The last restaurant displayed is Taco Bell.
Test 2: Chose remove Restaurant. Typed in "Blah". It told me there was so such restaurant and returned to the main menu. Chose remove, typed in "Subway", checked list of restaurants and Subwas was gone.
Test 3: Started the tournament by typing in 5.

Total Number of Rounds: 4
Remaining of Contestants: 16

Round 1 Match 1
Mimis Cafe or Olive Garden: Olive Garden

Round 1 Match 2
Macaroni Grill or Chipotle: Chipotle

Round 1 Match 3
Legends Grill or Brick Oven: Brick Oven

Round 1 Match 4
Carls Jr or TGI Fridays: TGI Fridays

Round 1 Match 5
Red Lobster or Carvers: Red Lobster

Round 1 Match 6
Cafe Rio or Costa Vita: Costa Vita

Round 1 Match 7
Subway or Panda Express: Panda Express

Round 1 Match 8
Fats Bistro or In-n-Out: Fats Bistro


Remaining of Contestants: 8

Round 2 Match 1
Olive Garden or Chipotle: Chipotle

Round 2 Match 2
Brick Oven or TGI Fridays: Brick Oven

Round 2 Match 3
Red Lobster or Costa Vita: Red Lobster

Round 2 Match 4
Panda Express or Fats Bistro: Panda Express


Remaining of Contestants: 4

Round 3 Match 1
Chipotle or Brick Oven: Chipotle

Round 3 Match 2
Red Lobster or Panda Express: Red Lobster


Remaining of Contestants: 2

Round 4 Match 1
Chipotle or Red Lobster: Chipotle


Our Winner: Chipotle
****************************************************/

/********************* Variables ********************/

double menu_option, total_rounds = 0;
string restaurants_array[] = {"In-n-Out", "Fats Bistro", "Panda Express", "Subway", "Costa Vita", "Cafe Rio", 
	"Carvers", "Red Lobster", "TGI Fridays", "Carls Jr", "Brick Oven", "Legends Grill", "Chipotle", "Macaroni Grill", "Olive Garden", "Mimis Cafe"};
//
vector< string > restaurants (restaurants_array, restaurants_array + (sizeof(restaurants_array)/sizeof(restaurants_array[0])));
bool repeat = true;

/****************************************************/

int main_menu(){
	cout << "\n\tMain Menu" << endl << "1. Display All Restaurants" << endl << "2. Add a Restaurant" << endl << "3. Remove a Restaurant" << endl << "4. Shuffle the Vector" << endl
		 << "5. Begin the Tournament" << endl << "6. Quit" << endl << "\nChoose Option: ";
	cin >> menu_option;
	
	while ( menu_option > 6 || menu_option < 1 ){
		cin.clear();
		cout << "\nPlease select valid option (1-6): ";
		cin >> menu_option;
	}
	return menu_option;
}

int list_restaurants(){
	int size = restaurants.size();
	for (int i = 0; i < size; i++){
		if (i < size - 1){
			cout << restaurants.at(i) << ", ";
		}
		else {
			cout << restaurants.at(i) << endl;
		}
	}
	return 0;
}

int add_restaurant(){
	string new_restaurant;
	int size = restaurants.size();
	int position_check = 0;
	bool already_exists = false;

	cout << "Restaurant Name: ";
	getline(cin, new_restaurant);
	for (int i = 0; i < size; i++){
		if (new_restaurant == restaurants.at(i)){
			cout << "That Restaurant is already on the list." << endl;
			already_exists = true;
		}
		else {
			position_check++;
		}
	}
	if (position_check == size){
		restaurants.push_back(new_restaurant);
		cout << new_restaurant << " has been added." << endl;
	}
	return 0;
}

int remove_restaurant(){
	string delete_me;
	int size = restaurants.size();
	int position_check = 0, position;
	bool already_exists = false;

	cout << "Restaurant Name: ";
	getline(cin, delete_me);
	for (int i = 0; i < size; i++){
		if (delete_me == restaurants.at(i)){
			already_exists = true;
			position = i;
		}
		else {
			position_check++;
		}
	}
	if (position_check == size && already_exists == false){
		cout << delete_me << " does not exist." << endl;
	}
	else if (already_exists == true){
		restaurants.erase(restaurants.begin()+position);
		cout << delete_me << " has been removed." << endl;
	}
	return 0;
}

int shuffle(){
	random_shuffle(restaurants.begin(), restaurants.end());
	return 0;
}

bool limit_check(){
	//make sure the number of restaurants is 2^n
	double i = restaurants.size();
	total_rounds = 0;
	for (i; i > 1; i = i/2){
		total_rounds++;
	}
	if( i == 1 ){
		return true;
	}
	else {
		return false;
	}
}

int battle(){
	cout << "Total Number of Rounds: " << total_rounds << endl;
	while(restaurants.size() > 1){
		for(int round = 1; round <= total_rounds; round++){
			//what happens in a round
			double contestants = pow(2, total_rounds+1-round);
			cout << "\n\nRemaining Contestants: " << contestants << endl;


			int match_number = 1;
			for(int match = pow(2, total_rounds-round); match >= 1 ; match--){
				//what happens in a single match
				cout << "\nRound " << round << "\tMatch " << match_number << endl;
				match_number++;

				string chosen_winner;
				bool valid_input = false;
				while( valid_input == false ){
					cout << restaurants.at((match*2)-1) << " or " << restaurants.at((match*2)-2) << ": ";
						
					getline(cin, chosen_winner);
					if (chosen_winner == restaurants.at((match*2)-2)){
						restaurants.erase(restaurants.begin()+(match*2)-1);
						valid_input = true;
					}
					else if (chosen_winner == restaurants.at((match*2)-1)){
						restaurants.erase(restaurants.begin()+(match*2)-2);
						valid_input = true;
					}
					else{
						cout << "Please choose one of the given restaurants...\n" << endl;
						valid_input = false;
					}
				}

			}

			/*

			int losers[16] = {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};		

			for(int match = 1; match <= pow(2, total_rounds-round); match++){
				//what happens in a single match
				cout << "\nRound " << round << "\tMatch " << match << endl;
						
				string chosen_winner;
				bool valid_input = false;
				while( valid_input == false ){
					cout << restaurants.at((match*2)-2) << " or " << restaurants.at((match*2)-1) << ": ";
						
					getline(cin, chosen_winner);
					if (chosen_winner == restaurants.at((match*2)-2)){
						//cout << restaurants.at((match*2)-1) << " has been removed" << endl;
						losers[match-1] = (match*2)-1;
						valid_input = true;
					}
					else if (chosen_winner == restaurants.at((match*2)-1)){
						//cout << restaurants.at((match*2)-2) << " has been removed" << endl;
						losers[match-1] = (match*2)-2;
						valid_input = true;
					}
					else{
						cout << "Please choose one of the given restaurants...\n" << endl;
						valid_input = false;
					}
				}
			}

			//remove all the entries in the loser array here
			for (int erase_me = (sizeof(losers)/sizeof(losers[0]))-1; erase_me >= 0; erase_me--){
				if(losers[erase_me] != 50){
					restaurants.erase(restaurants.begin()+losers[erase_me]);
				}
			} 
				
			*/
		}
	}
	cout << "\n\nOur Winner: " << restaurants.at(0) << endl;
	
	return 0;
}

int main(){
	while (repeat == true){
		system("CLS");
		main_menu();
		if (menu_option == 1){
			list_restaurants();
			system("pause");
			repeat = true;
		}
		else if (menu_option == 2){
			cin.ignore();
			add_restaurant();
			system("pause");
			repeat = true;
		}
		else if (menu_option == 3){
			cin.ignore();
			remove_restaurant();
			system("pause");
			repeat = true;
		}
		else if (menu_option == 4){
			shuffle();
			system("pause");
			repeat = true;
		}
		else if (menu_option == 5){
			if (limit_check() == true){
				cout << "Current Size: " << restaurants.size() << endl;
				cin.ignore();
				battle();
				system("pause");
				repeat = false;
			}
			else {
				cout << "Incorrect number of Restaraunts (" << restaurants.size() << ")" << endl;
				system("pause");
				repeat = true;
			}
		}
		else if (menu_option == 6){	
			repeat = false;
		}
	}
	return 0;
}
