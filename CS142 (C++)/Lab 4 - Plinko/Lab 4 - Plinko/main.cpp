#include <iostream>
#include <string>

using namespace std;

/************************************* TEST CASES *******************************************

	Test 1
	Mode:			Single
	Slot:			0
	Movements:		[0.5, 1, 0.5, 0, 0.5, 0, 0.5, 0, 0.5, 0, 0.5, 1]
	Prize:			$500

	Test 2
	Mode:			Single
	Slot:			8
	Movements:		[7.5, 8, 7.5, 8, 7.5, 7, 7.5, 7, 7.5, 7, 6.5, 7]
	Prize:			$500

	Test 3
	Mode:			Multiple
	Slot:			6
	Chips Dropped:	500
	Prize:			$907200
	Average Prize:	$1841


*************************************** VARIABLES ******************************************/

int menu_option, slot, end_slot, prize;
double current_slot;
bool repeat = true;

/*******************************************************************************************/

int main_menu()
{
	cout << "\n\n\tMain Menu" << endl << "1. Single Chip" << endl << "2. Multiple Chips" << endl << "3. Quit" << endl << "\nChoose Option: ";
	cin >> menu_option;
	
	while ( menu_option > 3 || menu_option < 1 )
	{
		cin.clear();
		cout << "\nPlease select valid option (1, 2 or 3): ";
		cin >> menu_option;
	}
	return menu_option;
}

int winnings(int end_slot)
{
	if ( end_slot == 0 )
		prize = 100;
	else if ( end_slot == 1 )
		prize = 500;
	else if ( end_slot == 2 )
		prize = 1000;
	else if ( end_slot == 3 )
		prize = 0;
	else if ( end_slot == 4 )
		prize = 10000;
	else if ( end_slot == 5 )
		prize = 0;
	else if ( end_slot == 6 )
		prize = 1000;
	else if ( end_slot == 7 )
		prize = 500;
	else if ( end_slot == 8 )
		prize = 100;

	return prize;
}

int chip_drop(int, bool show_end_slot, bool print_me)
{
	current_slot = slot;
	if (print_me == true)
		cout << "\nStarting Slot: " << slot << endl;
	for ( int i = 1; i <= 12; i++)
	{
		double movement = 0;
		double decider = (rand() % 2);
		if (current_slot == 0)
		{
			movement = 0.5;
		}
		else if (current_slot > 0 && current_slot < 8)
		{
			if (decider == 0)
				movement = -0.5;
			if (decider == 1)
				movement = 0.5;
		}
		else if (current_slot == 8)
		{
			movement = -0.5;
		}

		current_slot = current_slot + movement;
		
		if (print_me == true)
		{
			if (i == 1)
			{
				cout << "[" << current_slot << ", ";

			}
			if ( i >= 2 && i <= 11)
			{				
				cout << current_slot << ", ";
			}
			if (i == 12)
			{
				cout << current_slot << "]" << endl;
			}
		}
	}
	end_slot = current_slot;
	if (show_end_slot == true)
		cout << "\nEnding Slot: " << end_slot << endl;

	return end_slot;
}

int single()
{		
	cout << "Choose slot to drop chip into (0-8): ";
	cin >> slot;

	if (slot <= -1 || slot >= 9)
	{
		cout << "Sorry that input is invalid;" << endl;
	}
	else
	{
		chip_drop(slot, true, true);
		cout << "Amount Won: $" << winnings(end_slot) << "\n" << endl;
	}

	repeat = true;
	return 0;
}

int multiple()
{
	int total_winnings = 0;
	int number_of_chips = 0;

	cout << "Choose slot to drop chip into (0-8): ";
	cin >> slot;	

	if (slot <= -1 || slot >= 9)
	{
		cout << "Sorry that input is invalid;" << endl;
	}
	else
	{
		cout << "How many chips would you like to drop? ";
		cin >> number_of_chips;

		if (number_of_chips < 1)
		{
			cout << "Sorry you have to drop at least 1 chip" << endl;
		}
		else
		{
			for (int i = 1; i <= number_of_chips; i++)
			{
				chip_drop(slot, false, false);
				total_winnings = total_winnings + winnings(end_slot);
			}
			int average_winnings = total_winnings/number_of_chips;
			cout << "Average prize per chip: $" << average_winnings << endl;
			cout << "\nTotal Amount Won: $" << total_winnings << "\n" << endl;
		}
	}
		
	repeat = true;
	return 0;
}

int main()
{
	while (repeat == true)
	{
		system("CLS");
		cout << "\t\t\tWelcome to Plinko!";
		main_menu();

		if ( menu_option == 1 )
		{
			single();
			system("pause");
		}
		else if ( menu_option == 2 )
		{
			multiple();
			system("pause");
		}

		else if ( menu_option == 3 )
		{
			repeat = false;
			cout << "\nThanks for playing!" << endl;
			system("pause");
		}
	}
	return 0;
}