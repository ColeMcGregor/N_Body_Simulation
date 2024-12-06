/**
 * this file will create an input file for the simulation, based on chosen parameters
 * the input file will be saved in the same directory as the executable, with the name "input.txt"
 * 
 * if random is chosen, a random number of black holes, stars, planets, and moons will be chosen, and assigned to each other
 * if custom is chosen, the user will define the number of each body, and assign them to each other, or choose random assignment
 * if preset is chosen, the user will receive a version of the solar system, set to their real parameters
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

constexpr std::pair<double, double> STAR_MASS_RANGE = {1.5912e29, 3.0e32}; 
constexpr std::pair<double, double> PLANET_MASS_RANGE = {3.3e23, 4.7e27};
constexpr std::pair<double, double> MOON_MASS_RANGE = {7.5e15, 1.5e23};
constexpr std::pair<double, double> BLACKHOLE_MASS_RANGE = {6.0e30, 1.2e41};

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
