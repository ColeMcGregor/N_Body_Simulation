// Vector.h
#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>  // for std::ostream

class Vector {
public:
    double x, y, z;

    // Constructor declaration
    Vector();
    Vector(double x_, double y_, double z_);

    // Method declarations
    double magnitude() const;
    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    Vector operator*(double scalar) const;
    Vector operator/(double scalar) const;

    // Method declaration for print
    void print() const;

};

#endif
