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
 *      4. generateCustomRandomBodies
 *      5. generatePresetBodies
 *      6. generateUniqueRadius
 *      7. generateBoundedDouble
 *      8. calculateOrbitalPosition
 *      9. calculateOrbitalVelocity
 *      10. initiateHeavenscape
 * 3. main function
 */

#include <iostream>
#include <string>
#include <utility> // For std::pair
#include <vector>

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


/**
 * function used to generate radius of black holes, based on mass, as they have a defined radius based on their mass to be a black hole
 */
double generateSchwarzchildRadius(double mass) {
    return (2 * GRAVITATIONAL_CONSTANT * mass) / (SPEED_OF_LIGHT * SPEED_OF_LIGHT);
}

/**
 * generate random bodies, which will have random body counts, and random assignments of children
 * 
 * must input N, the total number of bodies
 * will have random assignment of bodies as children to each other, following hierarchy of mass
 *     will check that none of the children are more massive than the parent, and that no children are assigned to themselves, or more than one parent
 */
void generateRandomBodies() {
    int N, stars, planets, moons, blackHoles;
    cout << "Enter the total number of bodies (N): ";
    cin >> N;

    // Validate input
    if (N <= 0) {
        cerr << "Error: Number of bodies must be greater than 0." << endl;
        return;
    }

    // Generate random bodies (logic to be implemented)
    cout << "Generating " << N << " random bodies with random child assignments..." << endl;

    //generate random number of stars, planets, moons, and black holes, summed to N, with planets always being the most numerous
    //will assign random, bound values to each body, so that no planet is bigger than any star, and no moon bigger than any planet

     // Seed the random number generator
    srand(static_cast<unsigned int>(time(0)));

    // Determine the number of black holes (rare, typically 0-3)
    blackHoles = rand() % 4;

    // Determine the number of stars (10-20% of total bodies)
    stars = max(1, static_cast<int>(N * (0.1 + static_cast<double>(rand()) / RAND_MAX * 0.1))); // 10-20% range

    // Determine the number of planets (most numerous)
    planets = max(1, static_cast<int>(N * (0.4 + static_cast<double>(rand()) / RAND_MAX * 0.2))); // 40-60% range

    // Remaining bodies are moons
    moons = N - (blackHoles + stars + planets);

    // Ensure moons have at least 0 count
    moons = max(0, moons);

     // Output the generated counts
    cout << "Generated random distribution of bodies:\n";
    cout << "Black Holes: " << blackHoles << "\n";
    cout << "Stars: " << stars << "\n";
    cout << "Planets: " << planets << "\n";
    cout << "Moons: " << moons << endl;

    //now that we have the counts, we can make the random bodies, with default positions, velocities, and radii, to be set in initiateHeavenscape
    //we will also need to make a vector of used radii, to check against when generating unique radii
    //first generate the black holes
    if (blackHoles > 0) {
        for (int i = 0; i < blackHoles; i++) {
        }
    }

    
}

/**
 * generate custom bodies, which requires user to input numbers for bodies and numbers for each child assignment(should only be used for small systems)
 * 
 * must input N, the total number of bodies
 * must input the number of each type of body(star, planet, moon, black hole)
 * must input the number of children for each body( body number: children numbers, separated by spaces)
 *     will check that none of the children are more massive than the parent, and that no children are assigned to themselves, or more than one parent
 */
void generateCustomBodies() {
    int N;
    cout << "Enter the total number of bodies (N): ";
    cin >> N;

    if (N <= 0) {
        cerr << "Error: Number of bodies must be greater than 0." << endl;
        return;
    }

    cout << "Define the number of each type of body (star, planet, moon, black hole):" << endl;
    int stars, planets, moons, blackHoles;
    cout << "Stars: ";
    cin >> stars;
    cout << "Planets: ";
    cin >> planets;
    cout << "Moons: ";
    cin >> moons;
    cout << "Black holes: ";
    cin >> blackHoles;

    // Validate counts
    if (stars + planets + moons + blackHoles != N) {
        cerr << "Error: Total body count must match N." << endl;
        return;
    }

    // Custom child assignment (logic to be implemented)
    cout << "Define child assignments for each body..." << endl;
}

/**
 * generate custom random bodies, which will have custom body counts, but random assignments of children
 * 
 * must input N, the total number of bodies
 * must input the number of each type of body(star, planet, moon, black hole)
 * will have random assignment of bodies as children to each other, following hierarchy of mass
 *     will check that none of the children are more massive than the parent, and that no children are assigned to themselves, or more than one parent
 */
void generateCustomRandomBodies() {
    int N;
    cout << "Enter the total number of bodies (N): ";
    cin >> N;

    if (N <= 0) {
        cerr << "Error: Number of bodies must be greater than 0." << endl;
        return;
    }

    cout << "Define the number of each type of body (star, planet, moon, black hole):" << endl;
    int stars, planets, moons, blackHoles;
    cout << "Stars: ";
    cin >> stars;
    cout << "Planets: ";
    cin >> planets;
    cout << "Moons: ";
    cin >> moons;
    cout << "Black holes: ";
    cin >> blackHoles;

    if (stars + planets + moons + blackHoles != N) {
        cerr << "Error: Total body count must match N." << endl;
        return;
    }

    // Random child assignment (logic to be implemented)
    cout << "Randomly assigning children..." << endl;
}

/**
 * generate preset bodies, similar to the solar system
 * N = 297
 * 1 star, the sun
 * 8 planets, with moons
 *    -Mercury, no moons
 *    -Venus, no moons
 *    -Earth, 1 moon
 *    -Mars, 2 moons
 *    -Jupiter, 95 moons
 *    -Saturn, 146 moons
 *    -Uranus, 28 moons
 *    -Neptune, 16 moons
 * no black holes or other bodies
 */
void generatePresetBodies() {
    cout << "Generating preset system based on the Solar System..." << endl;

    // Predefined body setup (logic to be implemented)
    cout << "1 Star, 8 Planets, and their moons." << endl;
}

/**
 * @brief generates a unique radius for a body, will take in a vector of used radii to check against
 * @param minRadius the minimum radius of the body
 * @param maxRadius the maximum radius of the body
 * @param usedRadii the vector of radii that have already been used
 * @return the unique radius
 */
double generateUniqueRadius(double minRadius, double maxRadius, const std::vector<double> &usedRadii)
{
    double radius;
    bool unique;
    do
    {
        radius = minRadius + (maxRadius - minRadius) * ((double)rand() / RAND_MAX); // Random within bounds
        unique = true;                                                              // flag to check if the radius is unique
        for (double r : usedRadii)
        { // check if the radius is unique
            if (fabs(radius - r) < 1.0e6)
            {                   // avoid radii too close to others
                unique = false; // set the flag to false if the radius is not unique
                break;          // break out of the loop if the radius is not unique
            }
        }
    } while (!unique);
    return radius;
}

/**
 * @brief will generate a double value between a given range
 * @param minValue the minimum value of the range
 * @param maxValue the maximum value of the range
 * @return the double value
 */
double generateBoundedDouble(double minValue, double maxValue) {
    double value;
    //does not have to be unique, as it is used for radii, and not positions
    value = minValue + (maxValue - minValue) * ((double)rand() / RAND_MAX); // Random within bounds
    return value;
}

/**
 * @brief calculates the orbital position of a body, depending on the parent body's position and the orbital radius
 * @param parentPos the position of the parent body
 * @param orbitalRadius the orbital radius of the body(this is the distance from the parent body you want to be away from)
 * @return the orbital position of the body
 */
Vector calculateOrbitalPosition(const Vector &parentPos, double orbitalRadius)
{
    double theta = ((double)rand() / RAND_MAX) * 2.0 * M_PI; // Azimuthal angle [0, 2π]
    double phi = ((double)rand() / RAND_MAX) * M_PI;         // Inclination angle [0, π]

    double x = orbitalRadius * sin(phi) * cos(theta);       // x position
    double y = orbitalRadius * sin(phi) * sin(theta);       // y position
    double z = orbitalRadius * cos(phi);                    // z position

    return Vector(parentPos.x + x, parentPos.y + y, parentPos.z + z); // return the orbital position of the body
}

/**
 * @brief calculates the orbital velocity of a body, depending on the parent body's position and the orbital radius
 * @param parentPos the position of the parent body
 * @param childPos the position of the child body
 * @param parentMass the mass of the parent body
 * @param gravitationalMultiplier the gravitational multiplier of the simulation
 * @return the orbital velocity of the body
 */
Vector calculateOrbitalVelocity(const Vector &parentPos, const Vector &childPos, double parentMass, double gravitationalMultiplier)
{
    const double G = 6.67430e-11 * gravitationalMultiplier;

    // Calculate the distance vector and magnitude
    Vector r = childPos - parentPos;
    double distance = r.magnitude();

    // Orbital speed
    double speed = sqrt((G * parentMass) / distance);

    // Calculate a perpendicular velocity vector
    Vector unitR = r / distance;           // Unit vector of position
    Vector velocity(-unitR.y, unitR.x, 0); // Perpendicular in XY-plane

    // Normalize and scale to orbital speed
    velocity = velocity * speed;
    // return the orbital velocity of the body
    return velocity;
}

/**
 * @brief this algorithm is used to set the correct positions, velocities, and orbital velocities of the bodies
 * @details:
 * @param bodies the vector of bodies in the simulation
 * @param bodyCount the array of body counts ([0]: N, [1]: NS, [2]: NP, [3]: NM, [4]: NB)
 */
void initiateHeavenscape(vector<Body> &bodies, int bodyCount[5])
{
    /*
     * Determine the hierarchical structure and spatial arrangement of celestial bodies in the simulation:
     *
     * 1. Identify if black holes are present: (bodyCount[4] > 0)
     *    - If present, the black hole is(are) the heaviest body(ies) and serves as the center of the simulation.
     *    - Analyze the black hole's children (stars, planets, or both) and assign them appropriately.
     *    - the closest black hole to us is 1560 light years away, or 1.4759e+19 meters, which can be used to average the distance between black holes, if more than one
     *
     * 2. If no black holes are present: (bodyCount[4] == 0)
     *    - Designate the most massive body as the center of the simulation.
     *
     * 3. Arrange stars: (bodyCount[1] > 0)
     *    - Determine the number of stars and ensure sufficient spacing between them to accommodate their planetary systems.
     *    - Place stars in stable orbits around the black hole or the center of the simulation.
     *    - If multiple stars exist, ensure they are spaced apart to allow for planetary systems.
     *    - the closest star from sol is Alpha Centauri at 4.37 light years away, or 3.8 x 10^16 meters, which can be used to average the distance between stars
     *
     * 4. Assign planets: (bodyCount[2] > 0)
     *    - Determine the number of planets and assign each to the correct parent (star or black hole).
     *    - Ensure sufficient spacing between planets to allow for moon systems.
     *    - the closest planet to sol is Mercury at 57 million kilometers, or 5.7 x 10^10 meters, which can be used to bind the distance from a planet to a star
     *    - the shortest distance between any two planets is 0.28 AU, or 4.1887e+10, which can be used to average the minimum distance between planets
     *    - the longest distance between two planets in our solar system is 13.2 AU, or 1.9747e+12 meters, which can be used to average the maximum distance between planets around a star
     *
     * 5. Handle moons: (bodyCount[3] > 0)
     *    - Identify all moons and assign them to their appropriate parent planets.
     *    - Position moons after planets, ensuring their orbital radii are correctly assigned.
     *    - the closest moon to any planet in our solar system is phobos to mars, at an astonishly low 6000 km, or 6 x 10^6 meters, which can be used to average the minimum distance between moons and planets
     *    - the moon farthest from its host body is Neso from neptune, at 4.96 x 10^10 meters, which can be used to average the maximum distance between moons and planets
     *
     *  6. Special distribution cases (UNSTABLE INTERACTIONS)
     *    - The above bounds for the distances between bodies can be toggled off to create unstable interactions, this will result in the bodies spiraling into eachother, or out of the system
     *    - Additionally, the gravitional multiplier when not set at 1 will create unstable interactions, as the force of gravity will be too great or too small, causing the bodies to behave erratically
     *        - it should be noted that the bounds do not work at all for stable orbits when the gravitational multiplier is not 1.00
     *    - the stable flag in the input file can be toggled to allow for stable orbits when the gravitational multiplier is set to 1.00, otherwise it is irrelevant
     */
}

/**
 * main function, which will prompt the user for the mode, timestep, iterations, gravitational multiplier, and stability, and call the appropriate function
 */

int main() {
    string mode;

    // Prompt user for the mode
    cout << "Enter mode (random, custom, custom random, preset): ";
    getline(cin, mode); // Allow for "custom random" input

    // Validate mode
    if (mode != "random" && mode != "custom" && mode != "custom random" && mode != "preset") {
        cerr << "Invalid mode. Allowed modes are: random, custom, custom random, preset" << endl;
        return 1;
    }

    // Prompt user for timestep, iterations, gravitational multiplier, and stability
    double gravitationalMultiplier;
    int timestep, iterations;
    bool stable;
    cout << "Enter timestep(whole number): ";
    cin >> timestep;
    cout << "Enter iterations(whole number): ";
    cin >> iterations;
    cout << "Enter gravitational multiplier(1.0 for real world): ";
    cin >> gravitationalMultiplier;
    cout << "Enter stability (true or false)(if gravity 1.0, else irrelevant): ";
    cin >> stable;

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
    if (stable != true && stable != false) {
        cout << "Stability must be true or false, Enter Stability: " << endl;
        cin >> stable;
    }



        
    // Delegate tasks based on mode
    if (mode == "random") {
        generateRandomBodies();
    } else if (mode == "custom") {
        generateCustomBodies();
    } else if (mode == "custom random") {
        generateCustomRandomBodies();
    } else if (mode == "preset") {
        generatePresetBodies();
    }

    return 0;
}
