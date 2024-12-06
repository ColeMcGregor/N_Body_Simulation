#include "BlackHole.h"

BlackHole::BlackHole(double radius, double mass, Vector position, std::string type, double gravityMultiplier)
    : Body(radius, mass, position, type), gravityMultiplier(gravityMultiplier) {}