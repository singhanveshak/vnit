#include "ioom.h"
#include <iostream>
using namespace std;

int main()
{
    DynamicArray arr1;
    cout << "Default constructor: ";
    arr1.displayData();

    DynamicArray arr2(5);
    int data[] = {1, 2, 3, 4, 5};
    arr2.setData(data, 5);
    cout << "Parameterized constructor: ";
    arr2.displayData();

    DynamicArray arr3 = arr2;
    cout << "Copy constructor: ";
    arr3.displayData();

    data[0] = 10;
    cout << "After modifying original array: ";
    arr2.displayData();
    cout << "Copied array remains unchanged: ";
    arr3.displayData();

    return 0;
}
