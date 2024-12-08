#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
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

/**
 * Overload the << operator to output the vector to a file
 * @param file the file to output the vector to
 * @param vec the vector to output
 * @return the file with the vector appended(or written if it's a new file)
 */
std::ofstream& operator<<(std::ofstream& file, const Vector& vec) {
  file << std::to_string(vec.x) << ", " << std::to_string(vec.y) << ", " << std::to_string(vec.z); // <3
  file << std::endl;
  return file;
}

/**
 * Overload the << operator to output the vector to a file
 * @param file the file to output the vector to
 * @param vec the vector to output
 * @return the file with the vector appended(or written if it's a new file)
 */
std::ostream& operator<<(std::ostream& stream, const Vector& vec) {
  stream << std::to_string(vec.x) << ", " << std::to_string(vec.y) << ", " << std::to_string(vec.z); // <3
  stream << std::endl;
  return stream;
}

// Reset the vector to (0, 0, 0)
void Vector::reset() // 
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
