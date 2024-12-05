#include <cmath>
#include <iostream>
#include <fstream>
#include "vector.h"
#include "body.h"

// Constructor: Initialize with (x, y, z), defaults to (0, 0, 0)
Vector::Vector(double x_, double y_, double z_)
    : x(x_), y(y_), z(z_) {}

// Calculate magnitude (length of the vector)
double Vector::magnitude() const
{
  return std::sqrt((x * x) + (y * y) + (z * z));
}

// Vector addition
Vector Vector::operator+(const Vector &other) const
{
  return Vector(x + other.x, y + other.y, z + other.z);
}

// Vector subtraction
Vector Vector::operator-(const Vector &other) const
{
  return Vector(x - other.x, y - other.y, z - other.z);
}

// Scalar multiplication
Vector Vector::operator*(double scalar) const
{
  return Vector(x * scalar, y * scalar, z * scalar);
}

// Scalar division
Vector Vector::operator/(double scalar) const
{
  return Vector(x / scalar, y / scalar, z / scalar);
}

std::ofstream& operator<<(std::ofstream& file, const Vector& vec)
{
    file << vec.x << "," << vec.y << "," << vec.z;
    return file;
}
// Reset the vector to (0, 0, 0)
void Vector::reset()
{
  x = 0;
  y = 0;
  z = 0;
}

// Print the vector (x, y, z)
void Vector::print() const
{
  std::cout << x << "," << y << "," << z << std::endl;
}
