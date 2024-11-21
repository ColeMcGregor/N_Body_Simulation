/**
 * This file contains the implementation of the FileManager class, which is used to parse the input file for the creation of bodies in the simulation 
 * this is a stateless class, meaning that it does not maintain any internal state between calls to the loadConfig function
 * it is used to parse the input file and create the bodies in the simulation, and to output the bodies to a file
 * I AM A TOOL, NOT AN OBJECT
 * 
 * @requirements: input.txt file, must be in the same directory as the executable, and correctly formatted
 * @output: output.txt file, will be in the same directory as the executable, used to simulate visualizations of the bodies in openGL
 * 
 * @author: Cole McGregor, Hawk Lindner, Brandon Trama
 */

#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "body.h"
using namespace std;


/**
 * @brief This function is used to load the configuration file and parse the information to create the bodies in the simulation
 * @param filePath: the path to the input file
 * @param bodies: the vector(datastructure that acts as a dynamic array) of bodies to be created
 * @param timestep: the timestep of the simulation
 * @param iterations: the number of iterations of the simulation
 */
void FileManager::loadConfig(const string& filePath, vector<Body>& bodies, double& timestep, double& gravityMultiplier, int& iterations) {
    // open the file
    ifstream file(filePath);
    // check if the file is open
    if (!file.is_open()) {
        throw runtime_error("Unable to open file: " + filePath);
    }
    // read the file line by line
    string line;
    // while there is a line to read, read it
    while (getline(file, line)) {
        // create a stringstream object to parse the line
        stringstream ss(line);

        // Parse timestep and iterations
        //if the line contains the word "timestep", then parse the timestep
        if (line.find("timestep") != string::npos) {
            ss >> timestep;
        } //else if the line contains the word "iterations", then parse the iterations
        else if (line.find("iterations") != string::npos) {
            ss >> iterations;
        }
        else if (line.find("gravityMultiplier") != string::npos) {
            ss >> gravityMultiplier;
        }
        // Parse body data
        else if (line.find("body") != string::npos) {
            double x, y, z, vx, vy, vz, mass, radius, oblateness;
            string type;
            ss >> x >> y >> z >> vx >> vy >> vz >> mass >> radius >> oblateness >> type;

            // Create a Body object and add it to the vector
            //emplace_back is used to add a new element to the end of the vector, emplace_front is used to add a new element to the beginning of the vector
            bodies.emplace_back(Vector(x, y, z), Vector(vx, vy, vz), Vector(), Vector(), mass, 0, 0, 0, 0, radius, oblateness, type);
        }
    }

    file.close();
}
/**
 * @brief This function is used to output the locations of the bodies as the simulation runs, allowing for visualizations of the simulation
 * @param filePath: the path to the output file
 */
void FileManager::outputResults(const string& filePath) {
    // open the file
    ofstream file(filePath);
    // check if the file is open
    if (!file.is_open()) {
        throw runtime_error("Unable to open file: " + filePath);
    }
    // output the locations of the bodies to the file as the simulation runs(sets of "NValue x,y,z" coordinates)

}
