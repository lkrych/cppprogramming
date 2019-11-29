#include <iostream>

int main()
{
    int favorite_number;
    // << insertion operator
    // >> extraction operator
    std::cout << "Enter your favorite number between 1 and 100: ";
    
    std::cin >> favorite_number;

    std::cout << "Amazing, " << favorite_number << " is my favorite number too!" << std::endl;

    return 0;
}