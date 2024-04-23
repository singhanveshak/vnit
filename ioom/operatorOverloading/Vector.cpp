#include "Vector.h"
#include <iostream>

Vector::Vector() {}

Vector::Vector(const std::vector<double>& vals) : elements(vals) {}

Vector::Vector(const Vector& other) : elements(other.elements) {}

Vector Vector::operator=(const Vector& other) {
    if (this != &other) {
        elements = other.elements;
    }
    return *this;
}

Vector Vector::operator+(const Vector& other) const {
    if (elements.size() != other.elements.size()) {
        throw std::invalid_argument("Vectors must have the same size for addition.");
    }
    Vector result;
    for (size_t i = 0; i < elements.size(); ++i) {
        result.elements.push_back(elements[i] + other.elements[i]);
    }
    return result;
}

Vector Vector::operator-(const Vector& other) const {
    if (elements.size() != other.elements.size()) {
        throw std::invalid_argument("Vectors must have the same size for subtraction.");
    }
    Vector result;
    for (size_t i = 0; i < elements.size(); ++i) {
        result.elements.push_back(elements[i] - other.elements[i]);
    }
    return result;
}

Vector Vector::operator*(double scalar) const {
    Vector result;
    for (size_t i = 0; i < elements.size(); ++i) {
        result.elements.push_back(elements[i] * scalar);
    }
    return result;
}

double Vector::dot(const Vector& other) const {
    if (elements.size() != other.elements.size()) {
        throw std::invalid_argument("Vectors must have the same size for dot product.");
    }
    double result = 0.0;
    for (size_t i = 0; i < elements.size(); ++i) {
        result += elements[i] * other.elements[i];
    }          
    return result;
}

void Vector::display() const {
    std::cout << "(";
    for (size_t i = 0; i < elements.size(); ++i) {
        std::cout << elements[i];
        if (i != elements.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << ")" << std::endl;
}
