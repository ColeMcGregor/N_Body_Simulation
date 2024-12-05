#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class Body;

/*
    Vector struct for holding x,y,z coordinates,
    has a magnitude function, and a constructor that defaults to 0
    has operator overloading for +, -, *, and / to allow for vector math

    Vector is used to represent position, velocity, acceleration, and force
    as they are all vectored quantities


*/
struct Vector
{
    double x, y, z;
    std::vector<Body> relatedBodies;
    Vector(double x_ = 0.0, double y_ = 0.0, double z_ = 0.0);
    double magnitude() const;
    Vector operator+(const Vector &other) const;
    Vector operator-(const Vector &other) const;
    Vector operator*(double scalar) const;
    Vector operator/(double scalar) const;
    friend std::ofstream& operator<<(std::ofstream& file, const Vector& vec);
    void reset();
    void print() const;
};

#endif