#include "ioom.h"
#include <iostream>
#include <algorithm>
using namespace std;
DynamicArray::DynamicArray() : arr(nullptr), size(0) {}

DynamicArray::DynamicArray(int n) : size(n)
{
    arr = new int[size];
}

DynamicArray::DynamicArray(DynamicArray &other) : size(other.size)
{
    arr = new int[size];
    for(int i=0;i<size;i++){
        arr[i] = other.arr[i];
    }
    // copy(other.arr, other.arr + size, arr);
}

DynamicArray::~DynamicArray()
{
    delete[] arr;
}

void DynamicArray::setData(int *data, int n)
{
    delete[] arr;
    size = n;
    arr = new int[size];
    copy(data, data + size, arr);
}

void DynamicArray::displayData()
{
    cout << "Array elements: ";
    for (int i = 0; i < size; ++i)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int DynamicArray::getSize()
{
    return size;
}
