#include <iostream>
#include <string>

using namespace std;

/* 

				Test 1									Test 2										Test 3 

	Miles/Year:				5000			Miles/Year:				10000				Miles/Year:				15000
	Price of Gas:			4.0				Price of Gas:			2.0					Price of Gas:			3.5
	Hybrid Cost:			30000			Hybrid Cost:			23000				Hybrid Cost:			26500
	Hybrid MPG:				60				Hybrid MPG:				90					Hybrid MPG:				120
	Hybrid Resale:			25000			Hybrid Resale:			20000				Hybrid Resale:			21000
	Non-Hybrid Cost:		20000			Non-Hybrid Cost:		12000				Non-Hybrid Cost:		18600
	Non-Hybrid MPG:			30				Non-Hybrid MPG:			18					Non-Hybrid MPG:			35
	Non-Hybrid Resale:		15000			Non-Hybrid Resale:		6000				Non-Hybrid Resale:		15000

	Hybrid Total Gas:		416.667			Hybrid Total Gas:		555.556				Hybrid Total Gas:		625
	Hybrid Total Cost:		6666.67			Hybrid Total Cost:		4111.11				Hybrid Total Cost:		7687.50

	Non-Hybrid Total Gas:	833.333			Non-Hybrid Total Gas:	2777.78				Non-Hybrid Total Gas:	2142.86
	Non-Hybrid Total Cost:	8333.33			Non-Hybrid Total Cost:	11555.60			Non-Hybrid Total Cost:	11100.00

*/

int sign_check(double number, string text)
{	
	if (text == "\nMiles/Gallon (Hybrid): " || text == "\nMiles/Gallon (Non-Hybrid): ")
	{
		while (number < 1)
		{
			cin.clear();
			cout << "Error: Number can't be Zero or Negative.\n" << text;
			cin >> number;
		}
	}
	else 	
	{
		while (number < 0)
		{
			cin.clear();
			cout << "Error: Number is invalid.\n" << text;
			cin >> number;
		}
	}
	return 0;
}

/* 

New Normal Car: $15,000 and gets 25 mpg
Resale value after 5 years is 75% its original value
10,000 Miles driven each year
Gas Costs $2.50

What gas mileage would a new hybrid ($32,000) need to have in order 
for total costs for 5 years to be equal to the total costs for a normal car? 

Test different values in your program to find the correct answer. 

Answer: 166.667 MPG */

/************************************** VARIABLES ******************************************/

	double miles, gas_price;
	string criteria;

	string miles_text = "Estimated miles driven per year: ";
	string gas_price_text = "\nEstimated price for a gallon of gas: ";
	string hybrid_price_text = "\nCost of Hybrid Car: ";
	string hybrid_mpg_text = "\nMiles/Gallon (Hybrid): ";
	string hybrid_resale_text = "\nEstimated resale value after 5 years (Hybrid): ";
	string normal_price_text = "\nCost of Non-Hybrid Car: ";
	string normal_mpg_text = "\nMiles/Gallon (Non-Hybrid): ";
	string normal_resale_text = "\nEstimated resale value after 5 years (Non-Hybrid): ";
	string criteria_text = "\nBuying Criteria (Gas Cost / Total Cost): ";
	
	struct result{
		string title, gas_string, total_cost_text;
		double total_gas, total_cost, mpg, price, resale;
	} hybrid, normal;

/*******************************************************************************************/


int hybrid_method()
{
	double total_gas_local;
	double fuel_cost;
	double depreciation;

	cout << hybrid_price_text;
	cin >> hybrid.price;
	sign_check(hybrid.price, hybrid_price_text);

	cout << hybrid_mpg_text;
	cin >> hybrid.mpg;
	sign_check(hybrid.mpg, hybrid_mpg_text);	

	cout << hybrid_resale_text;
	cin >> hybrid.resale;
	sign_check(hybrid.resale, hybrid_resale_text);

	total_gas_local = (miles/hybrid.mpg)*5;
	fuel_cost = total_gas_local*gas_price;
	depreciation = hybrid.price - hybrid.resale;

	//cout << total_gas_local << endl;
	//cout << fuel_cost << endl;
	//cout << depreciation << endl;

	hybrid.title = "\n\n\t\tHybrid Results:\n\n";
	hybrid.gas_string = "Total Gallons of Gas used over 5 years: ";
	hybrid.total_gas = total_gas_local;
	hybrid.total_cost_text = "\nTotal Cost of the Car over 5 years: $"; 
	hybrid.total_cost = fuel_cost + depreciation;

	return 0;
}

int nonhybrid_method()
{
	double total_gas_local;
	double fuel_cost;
	double depreciation;
	
	cout << normal_price_text;
	cin >> normal.price;
	sign_check(normal.price, normal_price_text);

	cout << normal_mpg_text;
	cin >> normal.mpg;
	sign_check(normal.mpg, normal_mpg_text);

	cout << normal_resale_text;
	cin >> normal.resale;
	sign_check(normal.resale, normal_resale_text);

	total_gas_local = (miles/normal.mpg)*5;
	fuel_cost = total_gas_local*gas_price;
	depreciation = normal.price - normal.resale;

	//cout << total_gas_local << endl;
	//cout << fuel_cost << endl;
	//cout << depreciation << endl;

	normal.title = "\n\n\t\tNon-Hybrid Results:\n\n";
	normal.gas_string = "Total Gallons of Gas used over 5 years: ";
	normal.total_gas = total_gas_local;
	normal.total_cost_text = "\nTotal Cost of the Car over 5 years: $"; 
	normal.total_cost = fuel_cost + depreciation;

	return 0;
}

int hybrid_first()
{
	cout << hybrid.title << hybrid.gas_string << hybrid.total_gas << " Gallons" << hybrid.total_cost_text << hybrid.total_cost << endl;
	cout << normal.title << normal.gas_string << normal.total_gas << " Gallons" << normal.total_cost_text << normal.total_cost << endl;
	return 0;
}

int normal_first()
{
	cout << normal.title << normal.gas_string << normal.total_gas << " Gallons" << normal.total_cost_text << normal.total_cost << endl;
	cout << hybrid.title << hybrid.gas_string << hybrid.total_gas << " Gallons" << hybrid.total_cost_text << hybrid.total_cost << endl;
	return 0;
}

int main()
{
	string repeat = "y";
	while (repeat == "y")
	{
		cout << miles_text;
		cin >> miles;
		sign_check(miles, miles_text);

		cout << gas_price_text;
		cin >> gas_price;
		sign_check(gas_price, gas_price_text);

		hybrid_method();

		nonhybrid_method();

		cout << criteria_text;
		cin >> criteria;
	
		if (criteria == "Gas")
		{
			if (hybrid.mpg > normal.mpg)
				hybrid_first();
			else
				normal_first();
		}
		if (criteria == "Total")
		{
			if (hybrid.total_cost < normal.total_cost)
				hybrid_first();
			else
				normal_first();
		}

		cout << "\n\n" << endl;

		cout << "Repeat? (y/n)";
		cin >> repeat;
	}
	return 0;
}