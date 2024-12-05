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

    // You can now handle each mode appropriately
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
