#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

/************************************* TEST CASES *******************************************



*************************************** VARIABLES ******************************************/

bool repeat = true, stop = false;
double hot_plate[20][20], new_array[20][20];

/*******************************************************************************************/

int initializer()
{
	for(int r = 0; r <= 19; r++)
	{
		for(int c = 0; c <= 19; c++)
		{
			hot_plate[r][c] = 0;
		}
	}

	for(int c = 1; c <= 18; c++)
	{
		hot_plate[0][c] = 100;
		hot_plate[19][c] = 100;
	}


	for(int r = 0; r <= 19; r++)
	{
		for(int c = 0; c <= 19; c++)
		{
			new_array[r][c] = 0;
		}
	}

	for(int c = 1; c <= 18; c++)
	{
		new_array[0][c] = 100;
		new_array[19][c] = 100;
	}
	
	return 0;
}

int print_array()
{	
	ofstream output_text("lab_6_output.csv");
	for(int r = 0; r <= 19; r++)
	{
		for(int c = 0; c <= 19; c++)
		{
			cout << fixed << setprecision(2) << hot_plate[r][c] << "\t";
			output_text << hot_plate[r][c] << ","; 
		}
		cout << endl;
		output_text << endl;
	}
		
	return 0;
}

int calculate_new_array()
{
	for(int r = 1; r <= 18; r++)
	{
		for(int c = 1; c <= 18; c++)
		{
			new_array[r][c] = (hot_plate[r][c-1] + hot_plate[r-1][c] + hot_plate[r][c+1] + hot_plate[r+1][c])/4;
		}
	}

	return 0;
}

int save_new_array()
{
	for(int r = 0; r <= 19; r++)
	{
		for(int c = 0; c <= 19 ; c++)
		{
			hot_plate[r][c] = new_array[r][c];
		}
	}

	return 0;
}

int compare()
{	
	int number_true = 0;
	for(int r = 1; r <= 18; r++)
	{
		for(int c = 1; c <= 18 ; c++)
		{
			double difference = abs(new_array[r][c] - hot_plate[r][c]);
			if ( difference < .1 )
			{
				number_true++;
			}
		}
	}

	if (number_true >= 324)
		stop = true;

	return 0;
}

int main()
{
	initializer();
	while (repeat == true)
	{
		system("CLS");
		cout << "Hot Plate\n\n";

		print_array();
		while (stop == false)
		{
			calculate_new_array();
			compare();
			save_new_array();
		}
		
		cout << "\n\nArray is now Stable.\n" << endl;
		print_array();


		cout << "\n\n";
		system("pause");
	}
	return 0;
}