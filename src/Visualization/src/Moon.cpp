#include "Moon.h"

Moon::Moon(double radius, double mass, Vector position, std::string type, int planetID)
    : Body(radius, mass, position, type), planetID(planetID) {}
