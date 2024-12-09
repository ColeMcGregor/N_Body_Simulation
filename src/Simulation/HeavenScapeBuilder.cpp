/**
 * this file will create an input file for the simulation, based on chosen parameters
 * the input file will be saved in the same directory as the executable, with the name "input.txt"
 * 
 * if random is chosen, a random number of black holes, stars, planets, and moons will be chosen, and assigned to each other
 * if custom is chosen, the user will define the number of each body, and assign them to each other, or choose random assignment
 * if preset is chosen, the user will receive a version of the solar system, set to their real parameters
 * 
 * will also prompt the user for the timestep, iterations, gravitational multiplier, and stability
 * 
 * will validate inputs and reprompt if necessary
 * 
 * will output the input file, and the bodies to the console
 * 
 * for body generation, it will determine position, and orbital velocity based on the hierarchy of mass. 
 * 
 * when building out the orbits, it will have a hierarchically reduced allowed radius from parent, to ensure minimal overlap between levels
 * 
 * Table of contents:
 * 1. definitions for generating bodies
 * 2. function prototypes for body generation
 *      1. generateSchwarzchildRadius
 *      2. generateRandomBodies
 *      3. generateCustomBodies
 *      4. generatePresetBodies
 *      5. generateUniqueRadius
 *      6. generateBoundedDouble
 *      7. calculateOrbitalPosition
 *      8. calculateOrbitalVelocity
 *      9. initiateHeavenscape
 *      10. outputBodies
 * 3. main function
 */

#include <iostream>
#include <string>
#include <utility> // For std::pair
#include <vector>
#include "vector.h"
#include "body.h"
#include "HeavenScapeBuilder.h"

using namespace std;


/**
 * defintions for generating bodies, will be used throughout
 * 
 * will hold the minimum and maximum mass for each body type, to be used in random body generation, to limit unusual or impossible values
 * 
 * mass ranges are in kg, and are gleened from internet research, uncited...
 */

//mass ranges in kg
constexpr std::pair<double, double> STAR_MASS_RANGE = {1.5912e29, 3.0e32}; 
constexpr std::pair<double, double> PLANET_MASS_RANGE = {3.3e23, 4.7e27};
constexpr std::pair<double, double> MOON_MASS_RANGE = {7.5e15, 1.5e23};
constexpr std::pair<double, double> BLACKHOLE_MASS_RANGE = {6.0e30, 1.2e41};
//radius ranges in meters from parent bodies, for the hierarchy of orbital radii
constexpr std::pair<double, double> STAR_RADIUS_RANGE = {1.5e7, 2.3e15};
constexpr std::pair<double, double> PLANET_RADIUS_RANGE = {4.8e10, 2.0e12};
constexpr std::pair<double, double> MOON_RADIUS_RANGE = {6.0e5, 5.0e10};
constexpr std::pair<double, double> BLACKHOLE_RADIUS_RANGE = {1.5e19, 1.5e21};
//constants
const double GRAVITATIONAL_CONSTANT = 6.67430e-11;  //G baby
const double SPEED_OF_LIGHT = 2.99792458e8;       //meters per second

//global variables
int N, NB, NS, NP, NM;
double gravitationalMultiplier = 1.0;
int timestep;
int iterations;
int bodyCount[5];
vector<double> usedRadii;
vector<Body> bodies;

//fill in vectors for SUN constant
Vector SUN_POSITION(0.0, 0.0, 0.0);
Vector SUN_VELOCITY(0.0, 0.0, 0.0);
Vector SUN_ACCELERATION(0.0, 0.0, 0.0);
Vector SUN_NET_FORCE(0.0, 0.0, 0.0);

//Solar System constants
const Body SUN = Body(  SUN_POSITION, //position in center of system
                        SUN_VELOCITY, //velocity in center of system
                        SUN_ACCELERATION, //acceleration in center of system
                        SUN_NET_FORCE, //net force in center of system
                        1.989e30, //mass in kg
                        6.9634e8, //radius in meters    
                        gravitationalMultiplier, //gravitational multiplier
                        "star", //type
                        vector<int>{1, 2, 3, 4, 5, 6, 7, 8}, //children indices
                        vector<Vector>{} //trajectory
                        );


/**
 * function used to generate radius of black holes, based on mass, as they have a defined radius based on their mass to be a black hole
 */
// double generateSchwarzchildRadius(double mass) {
//     return (2 * GRAVITATIONAL_CONSTANT * mass) / (SPEED_OF_LIGHT * SPEED_OF_LIGHT);
// }

/**
 * generate random bodies, which will have random body counts, and random assignments of children
 * 
 * must input N, the total number of bodies
 * will have random assignment of bodies as children to each other, following hierarchy of mass
 *     will check that none of the children are more massive than the parent, and that no children are assigned to themselves, or more than one parent
 */
// void generateRandomBodies() {
//     int stars, planets, moons, blackHoles;
//     int currentBlackHoles, currentStars, currentPlanets, currentMoons;
//     cout << "Enter the total number of bodies (N): ";
//     cin >> N;

//     // Validate input
//     if (N <= 0) {
//         cerr << "Error: Number of bodies must be greater than 0." << endl;
//         return;
//     }

//     // Generate random bodies (logic to be implemented)
//     cout << "Generating " << N << " random bodies with random child assignments..." << endl;

//     //generate random number of stars, planets, moons, and black holes, summed to N, with planets always being the most numerous
//     //will assign random, bound values to each body, so that no planet is bigger than any star, and no moon bigger than any planet

//      // Seed the random number generator
//     srand(static_cast<unsigned int>(time(0)));

//     // Determine the number of black holes (rare, typically 0-3)
//     blackHoles = rand() % 4;

//     // Determine the number of stars (10-20% of total bodies)
//     stars = max(1, static_cast<int>(N * (0.1 + static_cast<double>(rand()) / RAND_MAX * 0.1))); // 10-20% range

//     // Determine the number of planets (most numerous)
//     planets = max(1, static_cast<int>(N * (0.4 + static_cast<double>(rand()) / RAND_MAX * 0.2))); // 40-60% range

//     // Remaining bodies are moons
//     moons = N - (blackHoles + stars + planets);

//     // Ensure moons have at least 0 count
//     moons = max(0, moons);

//      // Output the generated counts
//     cout << "Generated random distribution of bodies:\n";
//     cout << "Black Holes: " << blackHoles << "\n";
//     cout << "Stars: " << stars << "\n";
//     cout << "Planets: " << planets << "\n";
//     cout << "Moons: " << moons << endl;
//     cout << "Assigning bodies to bodyCount vector..." << endl;

//     //assign the body counts to the bodyCount vector
//     bodyCount[0] = N;
//     bodyCount[1] = blackHoles;
//     bodyCount[2] = stars;
//     bodyCount[3] = planets;
//     bodyCount[4] = moons;

//     //initialize the current body counters
//     currentBlackHoles = 0;
//     currentStars = 0;
//     currentPlanets = 0;
//     currentMoons = 0;

//     //check the bodyCount vector
//     cout << "Body Counts: N:" << bodyCount[0] << " NS:" << bodyCount[1] << " NP:" << bodyCount[2] << " NM:" << bodyCount[3] << " NB:" << bodyCount[4] << endl;
//     //inform user the bodies will be generated now
//     cout << "Generating bodies..." << endl; 

//     //now that we have the counts, we can make the random bodies, with default positions, velocities, and radii, to be set in initiateHeavenscape
//     //we will also need to make a vector of used radii, to check against when generating unique radii
//     //first generate the black holes
//     for (int i = 0; i < N; i++) {
//         // Default vectors factored out of the loop as every body will have the same default values
//         Vector position(0.0, 0.0, 0.0);
//         Vector velocity(0.0, 0.0, 0.0);
//         Vector acceleration(0.0, 0.0, 0.0);
//         Vector netForce(0.0, 0.0, 0.0);

//         //initialize the mass, radius, type, children indices, and trajectory for each body(must be local variables)
//         double mass, radius;
//         string type;
//         vector<int> childrenIndices;
//         vector<Vector> trajectory;

//         //will first do blackholes, then stars, then planets, then moons, because of the if ELSE logic
//         if (currentBlackHoles < blackHoles) {
//             // Generate Black Hole
//             mass = generateBoundedDouble(BLACKHOLE_MASS_RANGE.first, BLACKHOLE_MASS_RANGE.second);
//             radius = generateSchwarzchildRadius(mass);
//             type = "blackhole";
//             currentBlackHoles++;
//             cout << "Black Hole " << currentBlackHoles << " created." << endl;
//         } else if (currentStars < stars) {
//             // Generate Star
//             mass = generateBoundedDouble(STAR_MASS_RANGE.first, STAR_MASS_RANGE.second);
//             radius = generateUniqueRadius(STAR_RADIUS_RANGE.first, STAR_RADIUS_RANGE.second, usedRadii);
//             type = "star";
//             currentStars++;
//             cout << "Star " << currentStars << " created." << endl;
//         } else if (currentPlanets < planets) {
//             // Generate Planet
//             mass = generateBoundedDouble(PLANET_MASS_RANGE.first, PLANET_MASS_RANGE.second);
//             radius = generateUniqueRadius(PLANET_RADIUS_RANGE.first, PLANET_RADIUS_RANGE.second, usedRadii);
//             type = "planet";
//             currentPlanets++;
//             cout << "Planet " << currentPlanets << " created." << endl;
//         } else if (currentMoons < moons) {
//             // Generate Moon
//             mass = generateBoundedDouble(MOON_MASS_RANGE.first, MOON_MASS_RANGE.second);
//             radius = generateUniqueRadius(MOON_RADIUS_RANGE.first, MOON_RADIUS_RANGE.second, usedRadii);
//             type = "moon";
//             currentMoons++;
//             cout << "Moon " << currentMoons << " created." << endl;
//         }

//         // Add body to the list
//         bodies.push_back(Body(position, velocity, acceleration, netForce, mass, radius, gravitationalMultiplier, type, childrenIndices, trajectory));
//     }
//     cout << "All bodies generated." << endl;
//     cout << "Your new God will now create the universe..." << endl;

//     //children assignment
//     cout << "Assigning children to each body..." << endl;
//     //assign stars to black holes if there are black holes

//     //now we have our bodies, we can initiate the heavenscape
//     initiateHeavenscape(bodies, bodyCount);
// }

/**
 * generate custom bodies, which requires user to input numbers for bodies and numbers for each child assignment(should only be used for small systems)
 * 
 * must input N, the total number of bodies
 * must input the number of each type of body(star, planet, moon, black hole)
 * must input the number of children for each body( body number: children numbers, separated by spaces)
 *     will check that none of the children are more massive than the parent, and that no children are assigned to themselves, or more than one parent
 */
// void generateCustomBodies() {
//     cout << "Enter the total number of bodies (N): ";
//     cin >> N;

//     // Validate input for total bodies
//     if (N <= 0) {
//         cerr << "Error: Number of bodies must be greater than 0." << endl;
//         return;
//     }

//     // Variables to store counts of each type
//     int stars, planets, moons, blackHoles;
//     cout << "Define the number of each type of body (star, planet, moon, black hole):" << endl;

//     // Input for each body type
//     do {
//         cout << "Stars: ";
//         cin >> stars;
//         cout << "Planets: ";
//         cin >> planets;
//         cout << "Moons: ";
//         cin >> moons;
//         cout << "Black holes: ";
//         cin >> blackHoles;

//         if (stars + planets + moons + blackHoles != N) {
//             cerr << "Error: Total bodies must match N. Please re-enter." << endl;
//         }
//     } while (stars + planets + moons + blackHoles != N);

//     // Assign counts to global bodyCount array
//     bodyCount[0] = N;
//     bodyCount[1] = blackHoles;
//     bodyCount[2] = stars;
//     bodyCount[3] = planets;
//     bodyCount[4] = moons;

//     // Generate bodies with default positions and velocities
//     cout << "Generating bodies..." << endl;
//     for (int i = 0; i < N; ++i) {
//         double mass, radius;
//         string type;

//         // Determine body type and assign properties
//         if (i < blackHoles) {
//             mass = generateBoundedDouble(BLACKHOLE_MASS_RANGE.first, BLACKHOLE_MASS_RANGE.second);
//             radius = generateSchwarzchildRadius(mass);
//             type = "blackhole";
//         } else if (i < blackHoles + stars) {
//             mass = generateBoundedDouble(STAR_MASS_RANGE.first, STAR_MASS_RANGE.second);
//             radius = generateUniqueRadius(STAR_RADIUS_RANGE.first, STAR_RADIUS_RANGE.second, usedRadii);
//             type = "star";
//         } else if (i < blackHoles + stars + planets) {
//             mass = generateBoundedDouble(PLANET_MASS_RANGE.first, PLANET_MASS_RANGE.second);
//             radius = generateUniqueRadius(PLANET_RADIUS_RANGE.first, PLANET_RADIUS_RANGE.second, usedRadii);
//             type = "planet";
//         } else {
//             mass = generateBoundedDouble(MOON_MASS_RANGE.first, MOON_MASS_RANGE.second);
//             radius = generateUniqueRadius(MOON_RADIUS_RANGE.first, MOON_RADIUS_RANGE.second, usedRadii);
//             type = "moon";
//         }

//         // Add body to the list
//         bodies.push_back(Body(Vector(), Vector(), Vector(), Vector(), mass, radius, gravitationalMultiplier, type, {}, {}));
//     }

//     cout << "All bodies generated. Now define child relationships..." << endl;

//     // Assign children to each body
//     for (int i = 0; i < N; ++i) {
//         int numChildren;
//         cout << "Enter the number of children for body " << i << " (" << bodies[i].type << "): ";
//         cin >> numChildren;

//         if (numChildren > N - 1) {
//             cerr << "Error: Too many children specified. Skipping this body." << endl;
//             continue;
//         }

//         cout << "Enter the indices of the children (space-separated): ";
//         for (int j = 0; j < numChildren; ++j) {
//             int childIndex;
//             cin >> childIndex;

//             // Validate child index
//             if (childIndex == i || childIndex < 0 || childIndex >= N) {
//                 cerr << "Invalid child index. Skipping..." << endl;
//                 continue;
//             }
//             bodies[i].childrenIndices.push_back(childIndex);
//         }
//     }

//     // Assign positions and velocities
//     cout << "Assigning positions and velocities..." << endl;
//     initiateHeavenscape(bodies, bodyCount);

//     cout << "Custom bodies successfully generated and initialized." << endl;
// }


/**
 * generate preset bodies, similar to the solar system
 * N = 297
 * 1 star, the sun
 * 8 planets, with moons
 *    -Mercury, no moons
 *    -Venus, no moons
 *    -Earth, 1 moon
 *    -Mars, 2 moons(ignored for now)
 *    -Jupiter, 95 moons(ignored for now)
 *    -Saturn, 146 moons(ignored for now)
 *    -Uranus, 28 moons(ignored for now)
 *    -Neptune, 16 moons(ignored for now)
 * no black holes or other bodies
 */
void generatePresetBodies() {
    cout << "Generating preset system based on the Solar System..." << endl;
    // Solar System constants
    //orbital radii of the planets in meters: Sun, Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune
    vector<double> orbitalRadii = {0.0, 5.79e10, 1.08e11, 1.50e11, 2.28e11, 7.79e11, 1.43e12, 2.87e12, 4.50e12}; // Sun to Neptune
    //masses of the planets in kg: Sun, Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune
    vector<double> parentMasses = {1.989e30, 1.989e30, 1.989e30, 1.989e30, 1.989e30, 1.989e30, 1.989e30, 1.989e30}; // Sun for planets
    //planet mass ranges: Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune
    vector<double> planetMassRanges = {3.3011e23, 4.8675e24, 5.9722e24, 6.4171e23, 1.8982e27, 5.6834e26, 8.6810e25, 1.02413e26};
    //planet radius ranges: Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune
    vector<double> planetRadiusRanges = {2.4397e6, 6.0518e6, 6.3710e6, 3.3895e6, 6.9911e7, 5.8232e7, 2.5362e7, 2.4622e7};
    //vector of orbital Inclinations
    vector<double> orbitalInclinations = {0.0, 3.38, 3.86, 7.25, 5.65, 6.09, 5.51, 6.48, 11.88};
    //names of the bodies
    vector<string> bodyNames = {"Sun", "Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune"};

    // use the sun preset body
    bodies.push_back(SUN);
    cout << "Sun created." << endl;

    // Place the planets around the Sun(mercury to neptune)
    for (size_t i = 1; i < orbitalRadii.size(); ++i) {
        double r = orbitalRadii[i];
        double v = sqrt(GRAVITATIONAL_CONSTANT * parentMasses[i - 1] / r);
        //convert the inclination to radians
        double inclination = orbitalInclinations[i] * (M_PI / 180.0);
        //use the inclination to calculate the z position and velocity
        double z = r * sin(inclination);      // Z position based on inclination
        double vz = v * sin(inclination);    // Z velocity based on inclination

        // Calculate initial position and velocity
        Vector position(r, 0, z); // Start along X-axis
        Vector velocity(0, v, vz); // Perpendicular in Y-axis
        Vector acceleration(0, 0, 0); // Acceleration is zero
        Vector net_force(0, 0, 0); // Net force is zero

        // Create the planet
        Body planet(
            position, velocity, acceleration, net_force,
            planetMassRanges[i - 1], // Mass from range or preset
            planetRadiusRanges[i - 1], // Radius from range or preset
            gravitationalMultiplier, "planet", {}, {}
        );

        bodies.push_back(planet);
        cout << bodyNames[i] << " created at r = " << r << " with v = " << v << endl;
    }

    // Add Earth's Moon
    double moonR = 3.84e8; // Moon's distance from Earth in meters
    double moonV = sqrt(GRAVITATIONAL_CONSTANT * bodies[3].mass / moonR); // Use Earth's mass for moon calculation as parent mass

    Vector moonPosition(bodies[3].position.x + moonR, 0, 0); // Offset from Earth by any axis, in this case, x
    Vector moonVelocity(0, moonV + bodies[3].velocity.y, 0); // Tangential to Earth's velocity, in this case, y, to make a perpendicular velocity vector

    //acceleration and net force are zero, as the moon is not affected by any forces
    Vector moonAcceleration(0, 0, 0);
    Vector moonNetForce(0, 0, 0);

    Body moon(
        moonPosition, moonVelocity, moonAcceleration, moonNetForce,
        7.34767309e22, // Mass of moon
        1.7374e6, // Radius of moon
        gravitationalMultiplier, "moon", {}, {}
    );

    bodies.push_back(moon);
    cout << "Moon created at r = " << moonR << " from Earth with v = " << moonV << endl;

    // Output information
    cout << "Preset solar system generated with " << bodies.size() << " bodies." << endl;
}


/**
 * @brief generates a unique radius for a body, will take in a vector of used radii to check against
 * @param minRadius the minimum radius of the body
 * @param maxRadius the maximum radius of the body
 * @param usedRadii the vector of radii that have already been used
 * @return the unique radius
 */
// double generateUniqueRadius(double minRadius, double maxRadius, const std::vector<double> &usedRadii)
// {
//     double radius;
//     bool unique;
//     do
//     {
//         radius = minRadius + (maxRadius - minRadius) * ((double)rand() / RAND_MAX); // Random within bounds
//         unique = true;                                                              // flag to check if the radius is unique
//         for (double r : usedRadii)
//         { // check if the radius is unique
//             if (fabs(radius - r) < 1.0e6)
//             {                   // avoid radii too close to others
//                 unique = false; // set the flag to false if the radius is not unique
//                 break;          // break out of the loop if the radius is not unique
//             }
//         }
//     } while (!unique);
//     return radius;
// }

/**
 * @brief will generate a double value between a given range
 * @param minValue the minimum value of the range
 * @param maxValue the maximum value of the range
 * @return the double value
 */
// double generateBoundedDouble(double minValue, double maxValue) {
//     double value;
//     //does not have to be unique, as it is used for radii, and not positions
//     value = minValue + (maxValue - minValue) * ((double)rand() / RAND_MAX); // Random within bounds
//     return value;
// }

/**
 * @brief calculates the orbital position of a body, depending on the parent body's position and the orbital radius
 * @param parentPos the position of the parent body
 * @param orbitalRadius the orbital radius of the body(this is the distance from the parent body you want to be away from)
 * @return the orbital position of the body
 */
// Vector calculateOrbitalPosition(const Vector &parentPos, double orbitalRadius)
// {
//     double theta = ((double)rand() / RAND_MAX) * 2.0 * M_PI; // Azimuthal angle [0, 2π]
//     double phi = ((double)rand() / RAND_MAX) * M_PI;         // Inclination angle [0, π]

//     double x = orbitalRadius * sin(phi) * cos(theta);       // x position
//     double y = orbitalRadius * sin(phi) * sin(theta);       // y position
//     double z = orbitalRadius * cos(phi);                    // z position

//     return Vector(parentPos.x + x, parentPos.y + y, parentPos.z + z); // return the orbital position of the body
// }

/**
 * @brief calculates the orbital velocity of a body, depending on the parent body's position and the orbital radius
 * @param parentPos the position of the parent body
 * @param childPos the position of the child body
 * @param parentMass the mass of the parent body
 * @param gravitationalMultiplier the gravitational multiplier of the simulation
 * @return the orbital velocity of the body
 */
// Vector calculateOrbitalVelocity(const Vector &parentPos, const Vector &childPos, double parentMass, double gravitationalMultiplier)
// {
//     const double G = 6.67430e-11 * gravitationalMultiplier;

//     // Calculate the distance vector and magnitude
//     Vector r = childPos - parentPos;
//     double distance = r.magnitude();

//     // Orbital speed
//     double speed = sqrt((G * parentMass) / distance);

//     // Calculate a perpendicular velocity vector
//     Vector unitR = r / distance;           // Unit vector of position
//     Vector velocity(-unitR.y, unitR.x, 0); // Perpendicular in XY-plane

//     // Normalize and scale to orbital speed
//     velocity = velocity * speed;
//     // return the orbital velocity of the body
//     return velocity;
// }

/**
 * @brief this algorithm is used to set the correct positions, velocities, and orbital velocities of the bodies
 * @details:
 * @param bodies the vector of bodies in the simulation
 * @param bodyCount the array of body counts ([0]: N, [1]: NS, [2]: NP, [3]: NM, [4]: NB)
 */
// void initiateHeavenscape(vector<Body> &bodies, int bodyCount[5])
// {
//     const double DISTANCE_SCALING_FACTOR = 1.0e12; // Scaling factor for distances
//     const double MIN_DISTANCE = 1.0e10;           // Minimum separation between bodies

//     // Helper function to get parent position
//     auto getParentPosition = [&](int childIndex) -> Vector {
//         for (size_t i = 0; i < bodies.size(); ++i) {
//             for (int child : bodies[i].childrenIndices) {
//                 if (child == childIndex) {
//                     return bodies[i].position;
//                 }
//             }
//         }
//         return Vector(0, 0, 0); // Default to origin if no parent is found
//     };

//     // Black Holes
//     if (bodyCount[4] > 0) {
//         double blackHoleDistance = 1.4759e19; // Average distance between black holes
//         Vector center(0, 0, 0);

//         for (int i = 0; i < bodyCount[4]; ++i) {
//             double angle = 2.0 * M_PI * i / bodyCount[4]; // Spread black holes evenly
//             bodies[i].position = center + Vector(
//                 blackHoleDistance * cos(angle),
//                 blackHoleDistance * sin(angle),
//                 0);
//             bodies[i].velocity = Vector(0, 0, 0); // Black holes are stationary
//         }
//     }

//     // Stars
//     if (bodyCount[1] > 0) {
//         int startIndex = bodyCount[4]; // Start after black holes
//         int endIndex = startIndex + bodyCount[1];
//         double starDistance = 3.8e16; // Average distance from black holes to stars

//         for (int i = startIndex; i < endIndex; ++i) {
//             Vector parentPos = getParentPosition(i); // Get parent position (black hole)
//             double orbitalRadius = starDistance + (i - startIndex) * MIN_DISTANCE;

//             // Position star in orbit around its parent (black hole)
//             bodies[i].position = calculateOrbitalPosition(parentPos, orbitalRadius);

//             // Velocity for stable orbit
//             bodies[i].velocity = calculateOrbitalVelocity(
//                 parentPos, bodies[i].position, bodies[parentPos].mass, gravitationalMultiplier);
//         }
//     }

//     // Planets
//     if (bodyCount[2] > 0) {
//         int startIndex = bodyCount[4] + bodyCount[1]; // Start after black holes and stars
//         int endIndex = startIndex + bodyCount[2];
//         double planetDistance = 5.7e10; // Average distance between planets and stars

//         for (int i = startIndex; i < endIndex; ++i) {
//             Vector parentPos = getParentPosition(i); // Get parent position (star)
//             double orbitalRadius = planetDistance + (i - startIndex) * MIN_DISTANCE;

//             // Position planet in orbit around its parent (star)
//             bodies[i].position = calculateOrbitalPosition(parentPos, orbitalRadius);

//             // Velocity for stable orbit
//             bodies[i].velocity = calculateOrbitalVelocity(
//                 parentPos, bodies[i].position, bodies[parentPos].mass, gravitationalMultiplier);
//         }
//     }

//     // Moons
//     if (bodyCount[3] > 0) {
//         int startIndex = bodyCount[4] + bodyCount[1] + bodyCount[2]; // Start after black holes, stars, and planets
//         int endIndex = startIndex + bodyCount[3];
//         double moonDistance = 3.84e8; // Average distance between moons and planets

//         for (int i = startIndex; i < endIndex; ++i) {
//             Vector parentPos = getParentPosition(i); // Get parent position (planet)
//             double orbitalRadius = moonDistance + (i - startIndex) * MIN_DISTANCE;

//             // Position moon in orbit around its parent (planet)
//             bodies[i].position = calculateOrbitalPosition(parentPos, orbitalRadius);

//             // Velocity for stable orbit
//             bodies[i].velocity = calculateOrbitalVelocity(
//                 parentPos, bodies[i].position, bodies[parentPos].mass, gravitationalMultiplier);
//         }
//     }

//     cout << "Heavenscape initialization complete." << endl;
// }


/**
 * @brief outputs the bodies to the input.txt file
 * @param bodies the vector of bodies in the simulation
 */
void outputBodies() {
    ofstream outputFile("input.txt");
    /**
     * input format:
     * Timestep [timestep]
     * Iterations [iterations]
     * N [N]
     * NS [NS]
     * NP [NP]
     * NM [NM]
     * NB [NB]
     * gravitationalMultiplier [gravitationalMultiplier]
     * 
     * body [body number]
     * children [children indices]
     * position [x, y, z]
     * velocity [x, y, z]
     * mass [mass]
     * radius [radius]
     * [star, planet, moon, black hole]
     * ...
     */

    //output Timestep
    outputFile << "Timestep " << timestep << endl;
    //output Iterations
    outputFile << "Iterations " << iterations << endl;
    //output N
    outputFile << "N " << N << endl;
    //output NS
    outputFile << "NS " << NS << endl;
    //output NP
    outputFile << "NP " << NP << endl;
    //output NM
    outputFile << "NM " << NM << endl;
    //output NB
    outputFile << "NB " << NB << endl;
    //output gravitationalMultiplier
    outputFile << "gravitationalMultiplier " << gravitationalMultiplier << endl;
    //output each body
    for (size_t i = 0; i < bodies.size(); ++i) {
        outputFile << "body " << i << endl;
        outputFile << "children ";
        for (size_t j = 0; j < bodies[i].childrenIndices.size(); ++j) {
            outputFile << bodies[i].childrenIndices[j];
            if (j != bodies[i].childrenIndices.size() - 1) {
                outputFile << " "; // Add a space between children
            }
        }
        outputFile << endl; //end the line of children
        outputFile << "position ";
        outputFile << bodies[i].position << endl; //output position
        outputFile << "velocity ";
        outputFile << bodies[i].velocity << endl; //output velocity
        outputFile << "mass " << bodies[i].mass << endl; //output mass
        outputFile << "radius " << bodies[i].radius << endl; //output radius
        outputFile << bodies[i].type << endl; //output type
        outputFile << endl; //make empty line between bodies
    }
}

/**
 * main function, which will prompt the user for the mode, timestep, iterations, gravitational multiplier, and stability, and call the appropriate function
 */

int main() {
    string mode;

    // Prompt user for the mode
    cout << "Enter mode (preset): ";
    getline(cin, mode); // Allow for "custom random" input

    // Validate mode
    if (mode != "preset") {
        cerr << "Invalid mode. Allowed modes are: preset" << endl;
        return 1;
    }

    // Prompt user for timestep, iterations, gravitational multiplier, and stability
    cout << "Enter timestep(whole number): ";
    cin >> timestep;
    cout << "Enter iterations(whole number): ";
    cin >> iterations;
    cout << "Enter gravitational multiplier(1.0 for real world): ";
    cin >> gravitationalMultiplier;

    /**
     * validate inputs and reprompt if necessary
     */

    if (timestep <= 0) {
        cout << "Timestep must be greater than 0, Enter Timestep: " << endl;
        cin >> timestep;
    }
    if (iterations <= 0) {
        cout << "Iterations must be greater than 0, Enter Iterations: " << endl;
        cin >> iterations;
    }
    if (gravitationalMultiplier <= 0) {
        cout << "Gravitational multiplier must be greater than 0, Enter Gravitational Multiplier: " << endl;
        cin >> gravitationalMultiplier;
    }



        
    // Delegate tasks based on mode
    // if (mode == "random") {
    //     generateRandomBodies();
    //     if (bodies.size() != (N)) {
    //         cerr << "Error: Generating Random bodies failed." << endl;
    //         return 1;
    //     }
    // } else 
    if (mode == "preset") {
        generatePresetBodies();
        if (bodies.size() != static_cast<size_t>(N)) {
            cerr << "Error: Generating Preset bodies failed." << endl;
            return 1;
        }
        cout << "Preset system generated with " << bodies.size() << " bodies." << endl;
    }
    //output the bodies to the input.txt file
    outputBodies();
    //end the program
    return 0;
}
