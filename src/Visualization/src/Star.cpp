#include "Star.h"

Star::Star(double radius, double mass, const Vector& position, const std::string& type)
    : Body(radius, mass, position, type) {}