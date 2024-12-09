#ifndef BODY_H
#define BODY_H

#include <vector>
#include <string>
#include "vector.h" // Ensure you have the Vector class header available

class Body {
public:
    // Constructor for the Body class
    Body(int id, const std::string& type, double radius);

    // Method to update the positions of the body
    void updatePosition(const std::vector<Vector>& newPositions);

    // Getter for positions
    std::vector<Vector> getPosition() const;

    // Getter for body ID
    int getID() const;

    // Getter for body type
    std::string getType() const;

private:
    int id;                    // ID of the body
    std::string type;          // Type of the body (e.g., "star", "planet")
    double radius;             // Radius of the body
    std::vector<Vector> positions; // Positions of the body stored in a vector
};

#endif // BODY_H
