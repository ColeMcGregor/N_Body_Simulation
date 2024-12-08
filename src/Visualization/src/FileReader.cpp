#include "FileReader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <tuple>
#include "Body.h"
#include "Star.h"
#include "Planet.h"
#include "Moon.h"
#include "BlackHole.h"

using namespace std;

FileReader::FileReader(const std::string& fileName) : fileName(fileName) {}

int FileReader::readTimeStep() {
    std::ifstream file(fileName); // Open the file
    if (!file.is_open()) {
        throw runtime_error("Unable to open file: " + fileName);
    }

    std::string line;
    int timestep = 0; // Initialize timestep with a default value

    // Read lines until "Timestep:" is found
    if (std::getline(file, line)) {
        istringstream iss(line);
        string prefix;
        int value;

        if(iss >> prefix >> value){
            file.close();
            return value;
        }
        else{
            throw runtime_error("Invalid format in file");
        }
    }
}


/*
    Calls readlocalBodies which will return a tuple of vectors for type 
    body, star, planet,moon,blackhole
*/
tuple<vector<Body>, std::vector<Star>, std::vector<Planet>, std::vector<Moon>, std::vector<BlackHole> > FileReader::readBodies() {
    
    vector<Body> localBodies;
    vector<Star> localStars;
    vector<Planet> localPlanets;
    vector<Moon> localMoons;
    vector<BlackHole> localBH;
    ifstream file(fileName);

    int numBodies = 0;
    int numStars = 0;
    int numPlanets = 0;
    int numMoons = 0;
    int numBlackHoles = 0; 
    int timestep = 0;

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + fileName);
    }
    //variables that will be sent over
    double radius, mass, x, y, z;
    string type;


    //for moon
    int planetID; 

    //gives you the line of the file   
    std::string line;
    //while loop for reading in the file
    
    while (std::getline(file, line)) {
    // Use istringstream to parse the line
        std::istringstream iss(line);
        std::string prefix;
        int value;
        // Parse lines with radius, mass, position (x, y, z), and type
        double radius, mass, x, y, z;
        std::string type;

        // Attempt to parse the data
        if (iss >> radius >> mass >> x >> y >> z >> type) {
            if (type == "star") {
                // Create and push the Star object
                Star star(radius, mass, Vector(x, y, z), type);
                localStars.push_back(star);
                localBodies.push_back(star);
            } else if (type == "planet") {
                // Create and push the Planet object
                Planet planet(radius, mass, Vector(x, y, z), type);
                localPlanets.push_back(planet);
                localBodies.push_back(planet);
            } else if (type == "moon") {
                // Create and push the Moon object
                Moon moon(radius, mass, Vector(x, y, z), type);
                localMoons.push_back(moon);
                localBodies.push_back(moon);
            } else if (type == "blackhole") {
                // Create and push the BlackHole object
                BlackHole blackhole(radius, mass, Vector(x, y, z), type);
                localBH.push_back(blackhole);
                localBodies.push_back(blackhole);
            }
        }
    }
    

    file.close();
    //returns timestep, localBodies, localStars, planets, localMoons and localBH
    return make_tuple(localBodies , localStars, localPlanets, localMoons, localBH);
}


