#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "player.h"

using namespace std;

/********************* Test Cases *******************

Test 1:*********************************************************************
Choose Option: 2
Player Name: Tester
Tester has been created.

Choose Option: 1
Name: Tester
Wins: 0
Losses: 0
Draws: 0
Percentage: 0%

Test 2:*********************************************************************
Choose Option: 4

Name: Tester
Wins: 0
Losses: 0
Draws: 0
Percentage: 0%

Choose Option: 5
There are not enough players in the lineup to play a game

Test 3:*********************************************************************
Choose Option: 2
Player Name: Player B
Player B has been created.

Choose Option: 3
Player Name: Player B
Player B has been added to the lineup.

Choose Option: 4

Name: Tester
Wins: 0
Losses: 0
Draws: 0
Percentage: 0%

Name: Player B
Wins: 0
Losses: 0
Draws: 0
Percentage: 0%

Choose Option: 5

Tester threw Scissors
Player B threw Scissors

The match was a draw.

Choose Option: 1

Name: Tester
Wins: 0
Losses: 0
Draws: 1
Percentage: 0%

Name: Player B
Wins: 0
Losses: 0
Draws: 1
Percentage: 0%

****************************************************/


int main_menu(){
	double menu_option;

	cout << "\n\tMain Menu" << endl 
		<< "1. Show Players" << endl 
		<< "2. Add Player" << endl 
		<< "3. Add to Line-up" << endl 
		<< "4. Show Line-up" << endl
		<< "5. Fight" << endl 
		<< "6. Quit" << endl 
		<< "\nChoose Option: ";

	cin >> menu_option;
	
	while ( menu_option > 6 || menu_option < 1 ){
		cin.clear();
		cout << "\nPlease select valid option (1-6): ";
		cin >> menu_option;
	}
	return menu_option;
}

int show_players(vector <Player*>& players){
	if (players.size() == 0){
		cout << "\nSorry, there are no players yet.\n" << endl;
	}
	else {
		cout << "\n";
		for(int i = 0; i < players.size(); i++){
			Player* p = players[i];
			cout << p->toString() << endl;
		}
		cout << "\n";
	}
	
	return 0;
}

int add_player(vector <Player*>& players){
	string name;

	cout << "\nPlayer Name: ";
	cin.ignore();
	getline(cin, name);

	Player* new_player = new Player(name);

	bool exists = false;
	for (int i = 0; i < players.size(); i++){
		if (players[i]->getName() == name)
			exists = true;			
	}
	if (exists == false){
		players.push_back(new_player);
		cout << "\n" << new_player->getName() << " has been created.\n" << endl;
	}
	else {
		cout << "\nThat player already exists...\n" << endl;
	}

	return 0;
}

int add_to_lineup(vector <Player*>& players, vector <Player*>& lineup){
	string name;
	Player* contestant;

	cout << "\nPlayer Name: ";
	cin.ignore();
	getline(cin, name);

	Player new_player = Player(name);

	bool exists = false;
	for (int i = 0; i < players.size(); i++){
		if (players[i]->getName() == name){
			exists = true;	
			contestant = players[i];
		}
	}
	if (exists == true){
		lineup.push_back(contestant);
		cout << "\n" << contestant->getName() << " has been added to the lineup.\n" << endl;
	}
	else {
		cout << "\nThat player doesn't exists...\n" << endl;
	}

	return 0;
}

int show_lineup(vector <Player*>& lineup){
	if (lineup.size() == 0){
		cout << "\nSorry, there are no players in the lineup.\n" << endl;
	}
	else {
		cout << "\n";
		for(int i = 0; i < lineup.size(); i++){
			Player* p = lineup[i];
			cout << p->toString() << endl;
		}
		cout << "\n";
	}
	
	return 0;
}

int fight(vector <Player*>& lineup){

	if (lineup.size() >= 2){
		Player* first = lineup[0];
		Player* second = lineup[1];

		
		double result =3;
		double rock = 0, paper = 1, scissors = 2, first_win = 1, second_win = 2, draw = 0;

		//check it contestant is the same person
		if (first->getName() == second->getName()){
			first->Tie();
			first->getRPSThrow();
			cout << "\nBoth players are the same person! Automatic Draw\n" << endl;
		}
		else{			
			double first_throw = first->getRPSThrow();
			double second_throw = second->getRPSThrow();

			if (first_throw == rock && second_throw == rock){
				result = draw;
			}
			else if (first_throw == rock && second_throw == paper){
				result = second_win;
			}
			else if (first_throw == rock && second_throw == scissors){
				result = first_win;
			}
			else if (first_throw == paper && second_throw == rock){
				result = first_win;
			}
			else if (first_throw == paper && second_throw == paper){
				result = draw;
			}
			else if (first_throw == paper && second_throw == scissors){
				result = second_win;
			}
			else if (first_throw == scissors && second_throw == rock){
				result = second_win;
			}
			else if (first_throw == scissors && second_throw == paper){
				result = first_win;
			}
			else if (first_throw == scissors && second_throw == scissors){
				result = draw;
			}
			
			string first_throw_text;
			string second_throw_text;

			if (first_throw == 0)
				first_throw_text = "Rock";
			else if (first_throw == 1)
				first_throw_text = "Paper";
			else if (first_throw == 2)
				first_throw_text = "Scissors";

			if (second_throw == 0)
				second_throw_text = "Rock";
			else if (second_throw == 1)
				second_throw_text = "Paper";
			else if (second_throw == 2)
				second_throw_text = "Scissors";
		
			cout << "\n" << first->getName() << " threw " << first_throw_text << endl;
			cout << second->getName() << " threw " << second_throw_text << "\n" << endl;

			if (result == first_win){
				first->Won();
				second->Lost();
				cout << first->getName() << " is the winner!\n" << endl;
			}
			else if (result == second_win){
				first->Lost();
				second->Won();
				cout << second->getName() << " is the winner!\n" << endl;
			}
			else if (result == draw){
				first->Tie();
				second->Tie();
				cout << "The match was a draw.\n" << endl;
			}
		}
		
		lineup.erase(lineup.begin(),lineup.begin()+2);
	}
	else{
		cout << "There are not enough players in the lineup to play a game" << endl;
	}
	return 0;
}


int main(){
	double menu_option, menu;
	vector <Player*> players;
	vector <Player*> lineup;
	bool repeat = true;

	while (repeat == true){
		system("CLS");
		menu_option = main_menu();
		if (menu_option == 1){
			show_players(players);
			system("pause");
			repeat = true;
		}
		else if (menu_option == 2){
			add_player(players);
			system("pause");
			repeat = true;
		}
		else if (menu_option == 3){
			add_to_lineup(players, lineup);
			system("pause");
			repeat = true;
		}
		else if (menu_option == 4){
			show_lineup(lineup);
			system("pause");
			repeat = true;
		}
		else if (menu_option == 5){
			fight(lineup);
			system("pause");
			repeat = true;
		}
		else if (menu_option == 6){	
			repeat = false;
		}
	}
	return 0;
}
