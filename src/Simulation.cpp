/**
 * @file Simulation.cpp
 * @brief This file contains the implementation of the Simulation class, which is used to run the simulation
 * 
 * @algorithm: this simulation will take in an input file, parse the information, and create the bodies in the simulation
 * it will then assign openMP threads to handle each body, and each thread will handle the calculations for that body
 * each thread will also handle the output of that body to a file
 * 
 * @author: Cole McGregor, Hawk Lindner, Brandon Trama
 * @requirements: FileManager class, which is used to parse the input file for the creation of bodies in the simulation, and the output of the bodies to a file
 * @dependencies: body.cpp, filemanager.cpp
 */

#include <vector>
#include <string>
#include "Body.h" // Include your Body class header
#include "FileManager.h" // Include your FileManager class header

using namespace std;



