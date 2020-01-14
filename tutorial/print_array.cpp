// arrays as parameters
#include <iostream>
using namespace std;

void printarray (int arg[], int length) {
    for (int n = 0; n < length; n++){
        cout << arg[n] << " ";
    cout << "\n";
    }
}

int main()
{
    int arr[] = {5, 10, 15};
    printarray(arr, 3);
    return 0;
}
