#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Car.h"

using namespace std;

/********************* Variables ********************/

double menu_option, balance = 10000;
vector <Car> inventory;
bool repeat = true;


/****************************************************/

int main_menu(){
	cout << "\n\tMain Menu" << endl 
		<< "1. Show Current Inventory" << endl 
		<< "2. Show Current Balance" << endl 
		<< "3. Buy a Car" << endl 
		<< "4. Sell a Car" << endl
		<< "5. Paint a Car" << endl 
		<< "6. Load File" << endl 
		<< "7. Save File" << endl 
		<< "8. Quit" << endl 
		<< "\nChoose Option: ";

	cin >> menu_option;
	
	while ( menu_option > 8 || menu_option < 1 ){
		cin.clear();
		cout << "\nPlease select valid option (1-8): ";
		cin >> menu_option;
	}
	return menu_option;
}

int show_all(){
	if (inventory.size() == 0){
		cout << "\nSorry you don't own any cars.\n" << endl;
	}
	else {
		cout << "\n";
		for(int i = 0; i < inventory.size(); i++){
			Car c = inventory[i];
			cout << c.toString() << endl;
		}
		cout << "\n";
	}
	
	return 0;
}

int	show_balance(){
	cout << "\n\tCurrent Balance: $" << balance << "\n\n";
	return 0;
}

int buy_car(){
	string name;
	string color;
	double price;

	cout << "\nCar Name: ";
	cin >> name;
	cout << "\nCar Color: ";
	cin >> color;
	cout << "\nCar Price: $";
	cin >> price;

	Car new_car = Car(name, color, price);

	if (price <= balance){
		bool exists = false;
		for (int i = 0; i < inventory.size(); i++){
			if (inventory[i].getName() == name)
				exists = true;			
		}
		if (exists == false){
			inventory.push_back(new_car);
			balance = balance - price;
			cout << "\nCongratulations on the purchase of your new " << new_car.getName() << "!" << endl;
			cout << "It has now been added to your inventory\n" << endl;
		}
		else {
			cout << "\nOops! You already own that car...\n" << endl;
		}
	}
	else{
		cout << "\nOh No! You don't have enough money to buy that new " << new_car.getName() << " :(\n" << endl;
	}
	return 0;
}

int sell_car(){
	if (inventory.size() == 0){
		cout << "\nSorry you don't have any cars to sell. Please buy one first.\n" << endl;
	}
	else{
		string name;
		cout << "\nName of Car to sell: ";
		cin >> name;
		bool exists = false;
		for (int i = 0; i < inventory.size(); i++){
			if (inventory[i].getName() == name){
				balance = balance + inventory[i].getPrice();
				cout << "\nCongratulations on selling the " << inventory[i].getName() << endl;
				cout << "$" << inventory[i].getPrice() << " has been added to your balance.\n" << endl;
				inventory.erase(inventory.begin()+i);
				exists = true;
			}
		}
		
		if (exists == false){
			cout << "\nSorry you don't own that car\n" << endl;
		}
	}

	return 0;
}

int paint_car(){
	if (inventory.size() == 0){
		cout << "\nSorry you don't have any cars to paint. Please buy one first.\n" << endl;
	}
	else{
		string name, new_color;
		cout << "\nName of Car to paint: ";
		cin >> name;
		bool exists = false;

		for (int i = 0; i < inventory.size(); i++){
			if (inventory[i].getName() == name){				
				cout << "\nNew Color: ";
				cin >> new_color;
				inventory[i].paint(new_color);
				cout << "\nCongratulations on painting the " << inventory[i].getName() << endl;
				cout << "It is now worth $" << inventory[i].getPrice() << "\n\n" << endl;
				exists = true;
			}
		}		
		if (exists == false){
			cout << "\nSorry, you can't paint a car you don't own\n" << endl;
		}
	}

	return 0;
}

int load_file(){
	inventory.clear();	
	ifstream input_file;
	string file_name;
	cout << "Enter File Name: ";
	cin >> file_name;

	input_file.open(file_name);

	double new_balance;

	//balance
	input_file >> new_balance;

	balance = new_balance;
	cout << "\nNew Balance: $" << balance << endl;

	//each car
	while(!input_file.eof()){
		string name, color;
		double price;
		input_file >> name;
		cout << "\nName: " << name << endl;
		input_file >> color;
		cout << "Color: " << color << endl;
		input_file >> price;
		cout << "Price: " << price << endl;
		Car new_car = Car(name, color, price);
		inventory.push_back(new_car);
	}
	cout << "\n\n";

	return 0;
}

int save_file(){
	
	ofstream out_file;
	string file_name;
	cout << "Enter File Name: ";
	cin >> file_name;
	
	out_file.open(file_name);

	out_file << balance;
	
	for (int i = 0; i < inventory.size(); i++){
		out_file << "\n" << inventory[i].getName() << " " << inventory[i].getColor() << " " << inventory[i].getPrice();
	}

	out_file.close();

	cout << "\n\nInventory Saved\n\n";

	return 0;
}

int main(){
	while (repeat == true){
		system("CLS");
		main_menu();
		if (menu_option == 1){
			show_all();
			system("pause");
			repeat = true;
		}
		else if (menu_option == 2){
			show_balance();
			system("pause");
			repeat = true;
		}
		else if (menu_option == 3){
			buy_car();
			system("pause");
			repeat = true;
		}
		else if (menu_option == 4){
			sell_car();
			system("pause");
			repeat = true;
		}
		else if (menu_option == 5){
			paint_car();
			system("pause");
			repeat = true;
		}
		else if (menu_option == 6){
			load_file();
			system("pause");
			repeat = true;
		}
		else if (menu_option == 7){
			save_file();
			system("pause");
			repeat = true;
		}
		else if (menu_option == 8){	
			repeat = false;
		}
	}
	return 0;
}
