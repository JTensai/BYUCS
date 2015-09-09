#include <iostream>
#include <string>

using namespace std;

int main()
{
	int guests;
	int large;
	int medium;
	int small;
	float pi = 3.14159;
	float area_needed;
	float tip;
	float subtotal;
	float total;
	int rounded_total;

	cout << "Number of Guests: ";
	cin >> guests;

	large = guests / 7;
	medium = (guests - (large*7))/3;
	small = (guests - (large*7) - (medium*3));

	cout << "Pizza's Needed:" << endl;
	cout << "Large:\t" << large << endl;
	cout << "Medium:\t" << medium << endl;
	cout << "Small:\t" << small << endl;

	area_needed = (large*pi*100)+(medium*pi*64)+(small*pi*36);

	cout << "\nTotal Area of Pizza Needed:\t" << area_needed << " square inches" << endl;
	cout << "Amount of Pizza per Guest:\t" << area_needed/guests << " square inches" << endl;
	
	subtotal = (large*14.68)+(medium*11.48)+(small*7.28);
	cout << "\nSubtotal: $" << subtotal << endl;
	cout << "Percent of total as tip: ";
	cin >> tip;
	tip = 1 + (tip/100);
	
	total = subtotal*tip;
	rounded_total = total;

	if (total >= rounded_total+.5)
		rounded_total = rounded_total + 1;

	cout << "\nTotal Cost: $" << rounded_total << endl;
	cout << "\n\n" << endl;


	system("pause");
	return 0;
}