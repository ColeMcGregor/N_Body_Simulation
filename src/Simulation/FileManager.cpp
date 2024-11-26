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
#include "vector.h"
using namespace std;


/**
 * @brief This function is used to load the configuration file and parse the information to create the bodies in the simulation
 * @param filePath: the path to the input file
 * @param bodies: the vector(datastructure that acts as a dynamic array) of bodies to be created
 * @param timestep: the timestep of the simulation
 * @param iterations: the number of iterations of the simulation
 */
void FileManager::loadConfig(const string& filePath, vector<Body>& bodies, double& timestep, double& gravitationalMultiplier, int& iterations) {
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
        else if (line.find("gravitationalMultiplier") != string::npos) {
            ss >> gravitationalMultiplier;
        }
        // Parse body data
        else if (line.find("body") != string::npos) {
            double x, y, z, vx, vy, vz, mass, density, radius, oblateness;
            string type;
            ss >> x >> y >> z >> vx >> vy >> vz >> mass >> density >> radius >> oblateness >> gravitationalMultiplier >> type;

            // Create a Body object and add it to the vector
            //emplace_back is used to add a new element to the end of the vector, emplace_front is used to add a new element to the beginning of the vector
            bodies.emplace_back(Vector(x, y, z), Vector(vx, vy, vz), Vector(), Vector(), Vector(), mass, 0, 0, 0, density, radius, oblateness, gravitationalMultiplier, type);
        }
    }

    file.close();
}
/**
 * @brief This function is used to output the locations of the bodies as the simulation runs, allowing for visualizations of the simulation
 * @param filePath: the path to the output file
 */
void FileManager::outputResults(const string& filePath, const vector<Body>& bodies, double currentTimestep) {
    // open the file
    ofstream file(filePath);
    // check if the file is open
    if (!file.is_open()) {
        throw runtime_error("Unable to open file: " + filePath);
    }
    // output the locations of the bodies to the file as the simulation runs(sets of "NValue x,y,z" coordinates)

    /// output current timestep
    file << "Timestep: " << currentTimestep << endl;

    // output the locations of the bodies to the file
    for (const Body& body : bodies) {
        file << body.position.x << " " << body.position.y << " " << body.position.z << " "  // Position vector
     << body.velocity.x << " " << body.velocity.y << " " << body.velocity.z << " "  // Velocity vector
     << body.acceleration.x << " " << body.acceleration.y << " " << body.acceleration.z << " "  // Acceleration vector
     << body.angular_velocity.x << " " << body.angular_velocity.y << " " << body.angular_velocity.z << " "  // Angular velocity vector
     << body.mass << " "  // Mass
     << body.roll << " "  // Roll angle
     << body.pitch << " "  // Pitch angle
     << body.yaw << " "  // Yaw angle
     << body.density << " "  // Density
     << body.radius << " "  // Radius
     << body.oblateness << " "  // Oblateness
     << body.type;  // Type string
    }

    file.close();

}
