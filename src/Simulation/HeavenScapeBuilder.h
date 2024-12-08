#ifndef HEAVENSCAPE_GENERATOR_H
#define HEAVENSCAPE_GENERATOR_H

#include <vector>
#include "vector.h"
#include "body.h"

// Function prototypes for body generation
double generateUniqueRadius(double minRadius, double maxRadius, const std::vector<double> &usedRadii);
double generateBoundedDouble(double min, double max);
double generateSchwarzchildRadius(double mass);
Vector calculateOrbitalPosition(const Vector &parentPos, double orbitalRadius);
Vector calculateOrbitalVelocity(const Vector &parentPos, const Vector &childPos, double parentMass, double gravitationalMultiplier);
void initiateHeavenscape(std::vector<Body> &bodies, int bodyCount[5]);


// Function prototypes for Input.txt creation
void generateRandomBodies();
void generateCustomBodies();
void generatePresetBodies();
void outputBodies();

#endif // HEAVENSCAPE_GENERATOR_H
