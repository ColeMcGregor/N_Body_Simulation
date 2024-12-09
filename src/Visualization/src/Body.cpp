#include "Body.h"
#include "vector.h"

// Constructor for the Body class
Body::Body(int id, const std::string& type, double radius)
    : id(id), type(type), radius(radius) {}

// Method to update the positions of the body
void Body::updatePosition(const std::vector<Vector>& newPositions) {
    positions = newPositions;  // Assign the new positions to the body
}

// Getter for positions
std::vector<Vector> Body::getPosition() const {
    return positions;  // Return the stored positions of the body
}

// Getter for body ID
int Body::getID() const {
    return id;  // Return the ID of the body
}

// Getter for body type
std::string Body::getType() const {
    return type;  // Return the type of the body
}
