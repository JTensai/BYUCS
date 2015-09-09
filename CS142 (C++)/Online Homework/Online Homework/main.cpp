#include <iostream>
#include <string>

using namespace std;

int main()
{
    int input;
	int i = 0;
	int i2 = 0;
	int n;

	cin >> input;

	while (i < input)
	{
		i2 = 0;
		++i;
		
		while (i2 < i)
		{
			n = (i2/4);
			i2++;
			if (i2-(4*n) == 1)
				cout << "B";
			if (i2-(4*n) == 2)
				cout << "Y";
			if (i2-(4*n) == 3)
				cout << "U";
			if (i2-(4*n) == 4)
				cout << "!";
		}
		cout << endl;
	}
    
	system("pause");
    return 0;
}