// my first pointer
#include <iostream>
using namespace std;
int main () {
    int firstvalue, secondvalue;
    int * mypointer;
    mypointer = &firstvalue;
    *mypointer = 10; 
    mypointer = &secondvalue; 
    *mypointer = 20;
    cout << "firstvalue is " << firstvalue << endl;   // prints firstvalue is 10
    cout << "secondvalue is " << secondvalue << endl; // prints secondvalue is 20
    return 0;
}