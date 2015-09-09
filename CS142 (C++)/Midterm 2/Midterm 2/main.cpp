#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h> 

using namespace std;

/********************* Test Cases ********************
z This is a very clever test! = #$Xlmw$mw$e$ziv"$gpiziv$xiwx%


****************************************************/

/********************* Variables ********************/

double menu_option;
string text_input, keys_input;
vector <string> message;
vector <int> keys;
vector <int> ascii_message;
bool repeat = true;

/****************************************************/

int main_menu(){
	message.clear();
	ascii_message.clear();
	keys.clear();
	cout << "\n\tMain Menu" << endl 
		<< "0. Quit" << endl 
		<< "1. Encrypt text using the Caesar Cipher and a Key" << endl 
		<< "2. Decrypt text using the Caesar Cipher and a key" << endl 
		<< "3. Decrypt text using the Caesar Cipher without the key" << endl 
		<< "4. Encrypt text using the 'Super Cipher' and a key" << endl
		<< "5. Decrypt text using the 'Super Cipher' and a key" << endl 
		<< "6. Decrypt text using the 'Super Cipher' without the key" << endl 
		<< "\nChoose Option: ";
	cin.clear();
	cin >> menu_option;
	
	while ( menu_option > 6 || menu_option < 0 ){
		cin.clear();
		cout << "\nPlease select valid option (0-6): ";
		cin >> menu_option;
	}
	return menu_option;
}

int get_input_for_encrypt(){
	
	cin.ignore();

	cout << "\nEnter text to encrypt: ";
	getline(cin, text_input);

	int key;
	cout << "\nEnter key for encryption: ";
	cin >> key;
	keys.push_back(key);
		
	return 0;
}

int get_input_for_encrypt_many(){
		
	cin.ignore();

	cout << "\nEnter text to encrypt: ";
	getline(cin, text_input);

	int keynum;
	cout << "\nEnter number of keys to input: ";
	cin >> keynum;

	int key;
	cout << "\nEnter keys for encryption: ";
	for(int i = 1; i <= keynum; i++){
		cin >> key;
		keys.push_back(key);
    }
		
	return 0;
}

int get_input_for_decrypt(){
	
	cin.ignore();

	cout << "\nEnter the encrypted text: ";
	getline(cin, text_input);

	int key;
	cout << "\nEnter key to use for decryption: ";
	cin >> key;
	keys.push_back(key);
		
	return 0;
}

int get_input_for_decrypt_many(){
		
	cin.ignore();

	cout << "\nEnter the encrypted text: ";
	getline(cin, text_input);

	int keynum;
	cout << "\nEnter number of keys to use for decryption: ";
	cin >> keynum;

	int key;
	cout << "\nEnter keys to use for decryption: ";
	for(int i = 1; i <= keynum; i++){
		cin >> key;
		keys.push_back(key);
    }
		
	return 0;
}

int get_input_for_crack(){
	
	cin.ignore();

	cout << "\nEnter text to decrypt: ";
	getline(cin, text_input);
			
	return 0;
}

int encrypt(vector <int> k){
	//add each character of input to vector 'ascii_message' in ascii code
	for (int i = 0; i < text_input.size(); i++){
		int ascii = text_input[i];
		ascii_message.push_back(ascii);
	}
	
	// add keys to corresponding vector positions
	int key_num = 0;
	for (int i = 0; i < ascii_message.size(); i++){
		ascii_message.at(i) = ascii_message.at(i) + keys.at(key_num);
		if (key_num == keys.size()-1){
			key_num = 0;
		}
		else {
			key_num++;
		}
	}
	cout << "\n\nConverted Ints: ";
	for (int i = 0; i < ascii_message.size(); i++){
		// subtract 91 from any numbers greater than 122
		if (ascii_message.at(i) > 122){
			ascii_message.at(i) = ascii_message.at(i) - 91;
		}

		//print out each entry of vector 'ascii_message'
		
		if (i < ascii_message.size() - 1){
			cout << ascii_message.at(i) << " ";
		}
		else {
			cout << ascii_message.at(i) << endl;
		}

		//convert each ascii character back to a letter
		int ascii = ascii_message.at(i);
		string letter = "";
		letter += (char) ascii;
		message.push_back(letter);
	}
	
	//print out each entry of vector 'message'	
	cout << "\nDecrypted Message: ";
	for (int i = 0; i < message.size(); i++){
		cout << message.at(i);
	}
	cout << "\n\n\n";
	return 0;
}

int decrypt(vector <int> k){

	//add each character of input to vector 'ascii_message' in ascii code
	for (int i = 0; i < text_input.size(); i++){
		int ascii = text_input[i];
		ascii_message.push_back(ascii);
	}
	
	// subtract keys from corresponding vector positions
	int key_num = 0;
	for (int i = 0; i < ascii_message.size(); i++){
		ascii_message.at(i) = ascii_message.at(i) - keys.at(key_num);
		if (key_num == keys.size()-1){
			key_num = 0;
		}
		else {
			key_num++;
		}
	}

	cout << "\n\nConverted Ints: ";
	for (int i = 0; i < ascii_message.size(); i++){
		// add 91 to any numbers less than 32
		if (ascii_message.at(i) < 32){
			ascii_message.at(i) = ascii_message.at(i) + 91;
		}

		//print out each entry of vector 'ascii_message'
		
		if (i < ascii_message.size() - 1){
			cout << ascii_message.at(i) << " ";
		}
		else {
			cout << ascii_message.at(i) << endl;
		}

		//convert each ascii character back to a letter
		int ascii = ascii_message.at(i);
		string letter = "";
		letter += (char) ascii;
		message.push_back(letter);
	}
	
	//print out each entry of vector 'message'
	cout << "\nDecrypted Message: ";
	for (int i = 0; i < message.size(); i++){
		cout <<  message.at(i);
	}
	cout << "\n\n\n";
	return 0;
}

int decrypt_32(int num){
		
	// subtract keys from corresponding vector positions
	
	vector <int> keys_32 = ascii_message;

	for (int i = 0; i < keys_32.size(); i++){
		keys_32.at(i) = keys_32.at(i) - num;
	}
	
	message.clear();
	cout << "\n\nConverted Ints: ";
	for (int i = 0; i < keys_32.size(); i++){
		// sub 91 from any numbers greater than 122
		if (keys_32.at(i) < 32){
			keys_32.at(i) = keys_32.at(i) + 91;
		}

		//print out each entry of vector 'ascii_message'
		
		if (i < keys_32.size() - 1){
			cout << keys_32.at(i) << " ";
		}
		else {
			cout << keys_32.at(i) << endl;
		}

		//convert each ascii character back to a letter
		int ascii = keys_32.at(i);
		string letter = "";
		letter += (char) ascii;
		message.push_back(letter);
	}
	//print out each entry of vector 'message'
	cout << "\nDecrypted Message: ";
	for (int i = 0; i < message.size(); i++){
		cout <<  message.at(i);
	}
	cout << "\n\n\n";
	return 0;
}



int code_crack(){
	//add each character of input to vector 'ascii_message' in ascii code
	for (int i = 0; i < text_input.size(); i++){
		int ascii = text_input[i];
		ascii_message.push_back(ascii);
	}

	vector< vector<int> > counter;	
	cout << "\n";
	
	for (int i = 0; i < ascii_message.size(); i++){
		vector <int> current;
		int count = 0;
		bool exists = false;
		for (int j = 0; j < ascii_message.size(); j++){			
			if (ascii_message.at(j) == ascii_message.at(i)){
				count++;
			}
		}		
		current.push_back(ascii_message.at(i));
		current.push_back(count);
		//cycle through all on counter
		if (counter.size() == 0){
			counter.push_back(current);
		}
		else {
			int count2 = 0;
			for (int j = 0; j < counter.size(); j++){
				if (current[0] == counter[j][0])
					count2++;
			}
			if (count2 == 0){
				counter.push_back(current);
			}
		}

	}

	for (int i = 0; i < counter.size(); i++){
		int ascii = counter[i][0];
		string letter = "";
		letter += (char) ascii;
		cout << "\"" << letter << "\" which is ASCII " << counter[i][0] << " occurs " << counter[i][1] << " time(s)." << endl;
	}
	cout << "\n\n";

	int biggest = counter[0][1];
	int winner = counter[0][0];
	for (int i = 0; i < counter.size(); i++){
		if (counter[i][1] > biggest){
			biggest = counter[i][1];
			winner = counter[i][0];
		}
	}
	
	cout << biggest << "(" << winner << ")" << endl;

	decrypt_32(winner-32);

	decrypt_32(73);

	
	return 0;
}

int main(){
	while (repeat == true){
		system("CLS");
		main_menu();
		if (menu_option == 1){
			get_input_for_encrypt();
			encrypt(keys);
			system("pause");
			repeat = true;
		}
		else if (menu_option == 2){
			get_input_for_encrypt();
			decrypt(keys);
			system("pause");
			repeat = true;
		}
		else if (menu_option == 3){
			get_input_for_crack();
			code_crack();
			system("pause");
			repeat = true;
		}
		else if (menu_option == 4){
			get_input_for_encrypt_many();
			encrypt(keys);
			system("pause");
			repeat = true;
		}
		else if (menu_option == 5){
			get_input_for_decrypt_many();
			decrypt(keys);
			system("pause");
			repeat = true;
		}
		else if (menu_option == 6){
			get_input_for_decrypt_many();
			decrypt(keys);
			system("pause");
			repeat = true;
		}
		else if (menu_option == 0){	
			repeat = false;
		}
	}
	return 0;
}
