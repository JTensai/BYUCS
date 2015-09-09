#include <iostream>
#include <string>

using namespace std;

int main()
{
    string pos1;
    string pos2;
    string pos3;
    
    cin >> pos1;
    cin >> pos2;
    cin >> pos3;
    
    if (pos1 == pos2 && pos1 == pos3 && pos2 == pos3)
        cout << "Device is armed. Do not open!";
    else
        cout << "Device is not armed. You may proceed";
    system("pause");
    return 0;
}