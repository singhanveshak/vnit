#ifndef VECTOR_H
#define VECTOR_H

#include <vector>
#include <stdexcept>

class Vector {
private:
    std::vector<double> elements;

public:
    Vector();
    Vector(const std::vector<double>& vals);
    Vector(const Vector& other);
    Vector operator=(const Vector& other);
    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    Vector operator*(double scalar) const;
    double dot(const Vector& other) const;
    void display() const;
};

#endif // VECTOR_H
