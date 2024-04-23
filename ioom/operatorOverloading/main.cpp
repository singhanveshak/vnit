#include "Vector.h"
#include <iostream>

int main() {
    // Test Vector class
    Vector v1({1, 2, 3});
    Vector v2({4, 5, 6});

    // Vector addition, subtraction, scalar multiplication, and dot product
    Vector result = v1 + v2 - (v1 * 2) + (v2 * 3);
    double dotProduct = v1.dot(v2);

    // Display results
    std::cout << "Result of operations: ";
    result.display();
    std::cout << "Dot product of v1 and v2: " << dotProduct << std::endl;

    return 0;
}
