#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <iostream>

// Vector struct for holding and manipulating 3D coordinates
struct Vector {
    double x, y, z;

    // Constructor: Initialize with (x, y, z), defaults to (0, 0, 0)
    Vector(double x_ = 0.0, double y_ = 0.0, double z_ = 0.0)
        : x(x_), y(y_), z(z_) {}

    // Calculate magnitude (length of the vector)
    double magnitude() const {
        return std::sqrt((x * x) + (y * y) + (z * z));
    }

    // Vector addition
    Vector operator+(const Vector& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    // Vector subtraction
    Vector operator-(const Vector& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    // Scalar multiplication
    Vector operator*(double scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }

    // Scalar division
    Vector operator/(double scalar) const {
        return {x / scalar, y / scalar, z / scalar};
    }

    // Print the vector (x, y, z)
    void print() const {
        std::cout << x << "," << y << "," << z << std::endl;
    }
};

#endif // VECTOR_H