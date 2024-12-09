#include "Star.h"

// Star::Star(double radius, const Vector& position, const std::string& type)
//     : Body(radius, position, type) {}

Star::Star(int id, const string& type,double radius)
    : Body(id, type,radius){}