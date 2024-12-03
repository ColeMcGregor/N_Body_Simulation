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
        stringstream stringReader(line);

        // Parse timestep and iterations
        //if the line contains the word "timestep", then parse the timestep
        if (line.find("Timestep") != string::npos) {
            stringReader >> timestep;
        } //else if the line contains the word "iterations", then parse the iterations
        else if (line.find("Iterations") != string::npos) {
            stringReader >> iterations;
        }
        else if (line.find("Gravitational Multiplier") != string::npos) {
            stringReader >> gravitationalMultiplier;
        }
        // Parse body data
        else if (line.find("body") != string::npos) {
            double x, y, z, vx, vy, vz, mass, density, radius;
            string type;
            stringReader >> x >> y >> z >> vx >> vy >> vz >> mass >> density >> radius >> gravitationalMultiplier >> type;

            // Create a Body object and add it to the vector
            //emplace_back is used to add a new element to the end of the vector, emplace_front is used to add a new element to the beginning of the vector
            bodies.emplace_back(Vector(x, y, z), 
                                Vector(vx, vy, vz), 
                                Vector(), //acceleration
                                Vector(), //net force
                                mass, 
                                density, 
                                radius, 
                                gravitationalMultiplier, 
                                type);
        }
    }

    file.close();
}
/**
 * @brief This function is used to output the locations of the bodies as the simulation runs, allowing for visualizations of the simulation
 * @param filePath: the path to the output file
 */
void FileManager::outputResults(const string& filePath, const vector<Body>& bodies, double timeStep) {
    // open the file
    ofstream file(filePath);
    // check if the file is open
    if (!file.is_open()) {
        throw runtime_error("Unable to open file: " + filePath);
    }
    // output the locations of the bodies to the file as the simulation runs(sets of "NValue x,y,z" coordinates)

    //create throwaway variables for the number of stars, planets, and moons
    int NS = 0; //number of stars
    int NP = 0; //number of planets
    int NM = 0; //number of moons
    int NB = 0; //number of blackholes
    //look through the bodies vector and count the number of stars, planets, and moons
    for (int i = 0; i < bodies.size(); i++) {
        if (bodies[i].type == "star") {
            NS++;
        }
        else if (bodies[i].type == "planet") {
            NP++;
        }
        else if (bodies[i].type == "moon") {
            NM++;
        }
        else if (bodies[i].type == "blackhole") {
            NB++;
        }
    }
    /// output current timestep
    file << "Timestep: " << timeStep << endl;                                // Timestep: 1
    file << "N: " << bodies.size() << endl;                                         // N: 10    
    file << "NS: " << NS << endl;                                                     // NS: 1
    file << "NP: " << NP << endl;                                                     // NP: 1
    file << "NM: " << NM << endl;                                                     // NM: 1
    file << "NB: " << NB << endl;                                                     // NB: 1
    for (int i = 0; i < bodies.size(); i++) {
        if (bodies[i].childrenIndices.size() > 0) {                                 // if the body has children
            file << i << " ";                                                       // output the index of the body
            for (int j = 0; j < bodies[i].childrenIndices.size(); j++) {            // output the indices of the children of the body
                file << bodies[i].childrenIndices[j] << " ";
            }
            file << endl;                                                           
        }
    }
    //output block style trajectories 
    for (int i = 0; i < bodies.size(); i++) {                                       // for each body
        // body number, body type, body radius
        file << i << " " << bodies[i].type << " " << bodies[i].radius << endl;      //output the body number, type, and radius
        for (int j = 0; j < bodies[i].trajectory.size(); j++) {                     // for each timestep
            file << bodies[i].trajectory[j].print() << endl;                        // output the trajectory of the body
        }
    }

    // output the locations of the bodies to the file

    file.close();

}
