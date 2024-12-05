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
#include <vector>

using namespace std;


/**
 * generate random bodies, which will have random body counts, and random assignments of children
 * 
 * must input N, the total number of bodies
 * will have random assignment of bodies as children to each other, following hierarchy of mass
 *     will check that none of the children are more massive than the parent, and that no children are assigned to themselves, or more than one parent
 */
void generateRandomBodies() {

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

}

int main(int argc, char *argv[]) {
    // Check the number of arguments
    if (argc < 2 || argc > 3) {
        cerr << "Usage: " << argv[0] << " <mode> [random children option]" << endl;
        cerr << "Modes: random, custom, custom random, preset" << endl;
        return 1;
    }

    // Parse the first argument
    string mode = argv[1];
    string secondWord = argc == 3 ? argv[2] : "";

    // Combine mode and second word if provided via concatenation
    if (!secondWord.empty()) {
        mode += " " + secondWord;
    }

    // Validate mode
    if (mode != "random" && mode != "custom" && mode != "custom random" && mode != "preset") {
        cerr << "Invalid mode. Allowed modes are: random, custom, custom random, preset" << endl;
        return 1;
    }

    // Assign the mode to a local variable
    cout << "Mode selected: " << mode << endl;

    /**
     * each of these will call a separate function to generate the input file
     */
    if (mode == "random") {
        cout << "Random mode selected. Generating random bodies..." << endl;
        // Add logic for random mode
    } else if (mode == "custom") {
        cout << "Custom mode selected. Awaiting user input..." << endl;
        // Add logic for custom mode
    } else if (mode == "custom random") {
        cout << "Custom Random mode selected. Partially randomized setup..." << endl;
        // Add logic for custom random mode
    } else if (mode == "preset") {
        cout << "Preset mode selected. Loading predefined system..." << endl;
        // Add logic for preset mode
    }

    return 0;
}
