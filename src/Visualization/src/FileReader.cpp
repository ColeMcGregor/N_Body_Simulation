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

FileReader::FileReader(const std::string& fileName) : fileName(fileName) {}


/*
    Calls readBodies which will return a tuple of vectors for type 
    body, star, planet,moon,blackhole
*/
tuple<vector<Body>, std::vector<Star>, std::vector<Planet>, std::vector<Moon>, std::vector<BlackHole> > FileReader::readBodies() {
    
    vector<Body> bodies;
    vector<Star> stars;
    vector<Planet> planets;
    vector<Moon> moons;
    vector<BlackHole> blackholes;
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
        //calls the iss in from line, returns the line , string takes the prefix, and int is the value
        // istringstream iss(line);
        // string prefix;
        // int value;

        //looks for "Timestep: int value or NB or NS or NP or NM or NBH: with int following them all"
        while (std::getline(file, line)) {
    // Use istringstream to parse the line
    std::istringstream iss(line);
    std::string prefix;
    int value;

    // Parse lines with a prefix and an integer value
    // if (iss >> prefix >> value) {
    //     if (prefix == "Timestep:") {
    //         timestep = value;
    //         cout<<"TImestep: " <<timestep<<endl;
    //     } else if (prefix == "NB:") {
    //         numBodies = value;
    //         cout<<"NB: "<<numBodies<<endl;
    //     } else if (prefix == "NS:") {
    //         numStars = value;
    //         cout<<"NS: "<<numStars<<endl;
    //     } else if (prefix == "NP:") {
    //         numPlanets = value;
    //         cout<<"NP: "<<numPlanets<<endl;
    //     } else if (prefix == "NBH:") {
    //         numBlackHoles = value;
    //         cout<<"NBH: " <<numBlackHoles<<endl;
    //     }
    // } 
    // Parse lines with radius, mass, position (x, y, z), and type
        double radius, mass, x, y, z;
        std::string type;

        // Attempt to parse the data
        if (iss >> radius >> mass >> x >> y >> z >> type) {
            if (type == "star") {
                // Create and push the Star object
                Star star(radius, mass, Vector(x, y, z), type);
                stars.push_back(star);
                bodies.push_back(star);
            } else if (type == "planet") {
                // Create and push the Planet object
                Planet planet(radius, mass, Vector(x, y, z), type);
                planets.push_back(planet);
                bodies.push_back(planet);
            } else if (type == "moon") {
                // Create and push the Moon object
                Moon moon(radius, mass, Vector(x, y, z), type);
                moons.push_back(moon);
                bodies.push_back(moon);
            } else if (type == "blackhole") {
                // Create and push the BlackHole object
                BlackHole blackhole(radius, mass, Vector(x, y, z), type);
                blackholes.push_back(blackhole);
                bodies.push_back(blackhole);
            }
        }
    }
    }
    
    file.close();
    //returns timestep, bodies, stars, planets, moons and blackholes
    return make_tuple(bodies , stars, planets, moons, blackholes);
}


