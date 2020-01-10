// cin with strings
#include <iostream>
#include <string>
using namespace std;

int main()
{
	string mystr;
	cout << "What's your name? ";
	getline (cin, mystr);
	cout << "Hello " << mystr << ".\n";
	cout << "What's your favorite color? ";
	getline (cin, mystr);
	cout << "Ugh... I hate " << mystr << ".\n";
	return 0;
}

