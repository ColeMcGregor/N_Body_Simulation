#include "Vector.h"
#include <cmath>  // For std::sqrt

// Default constructor (no arguments)
Vector::Vector() : x(0), y(0), z(0) {}

// Constructor with parameters
Vector::Vector(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

// Magnitude definition
double Vector::magnitude() const {
    return std::sqrt((x * x) + (y * y) + (z * z));
}

// Other operator definitions
Vector Vector::operator+(const Vector& other) const {
    return {x + other.x, y + other.y, z + other.z};
}

Vector Vector::operator-(const Vector& other) const {
    return {x - other.x, y - other.y, z - other.z};
}

Vector Vector::operator*(double scalar) const {
    return {x * scalar, y * scalar, z * scalar};
}

Vector Vector::operator/(double scalar) const {
    return {x / scalar, y / scalar, z / scalar};
}

// print method definition
void Vector::print() const {
    std::cout << x << ", " << y << ", " << z << std::endl;
}
