#include <iostream>
using namespace std;

ofstream myfile; //object construction
myfile.open(filename, mode) //stream opening
if (myfile.is_open()) { /* ok, proceed with output */ }