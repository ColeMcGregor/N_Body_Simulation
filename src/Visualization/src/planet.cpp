#include "Planet.h"

Planet::Planet(double radius, double mass, Vector position, std::string type, std::string material)
    : Body(radius, mass, position, type), material(material) {}
