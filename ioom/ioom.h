
class DynamicArray
{
private:
    int *arr; // Pointer to dynamically allocated integer array
    int size; // Size of the array
public:
    DynamicArray();
    DynamicArray(int n);
    DynamicArray( DynamicArray &other);
    ~DynamicArray();
    void setData(int *data, int n);
    void displayData();
    int getSize();
};