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

tuple<vector<Body>, std::vector<Star>, std::vector<Planet>, std::vector<Moon>, std::vector<BlackHole> > FileReader::readBodies() {
    vector<Body> bodies;

    vector<Star> stars;
    vector<Planet> planets;
    vector<Moon> moons;
    vector<BlackHole> blackholes;
    ifstream file(fileName);

    int count = 0; //once it reaches
    int numBodies = 0;
    int numStars = 0;
    int numPlanets = 0;
    int numMoons = 0;
    int numBlackHoles = 0; 
    int timestep = 0;

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + fileName);
    }

    double radius, mass, x, y, z;
    string type;

    //black hole var
    double gravityuMultiplier;

    //planet var
    string material;

    //for moon
    int planetID;

    //for star
    double luminosity;      
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        
        istringstream iss(line);
        string prefix;
        int value;

        if(iss >> prefix >> value){
            if (prefix == "Timestep: "){
                timestep = value;
            }
            else if (prefix == "NB: "){
                numBodies = value;
            }
            else if (prefix == "NS: "){
                numStars = value;
            }
            else if (prefix == "NP: "){
                numPlanets = value;
            }
            else if (prefix == "NBH: "){
                numBlackHoles = value;
            }
            else{
                cerr << "Unknown Prefix " <<prefix << endl;
            }

        }
        else if (!(iss >> radius >> mass >> x >> y >> z >> type)) {
            throw std::runtime_error("Invalid data format in file: " + fileName);
        }

        else if(type == "star"){
            if(!(iss >> luminosity))
            {
                throw runtime_error("Invalid data format for star in file: " +fileName);
            }
            else{
                Star star(radius,mass,Vector(x,y,z),type,luminosity);
                stars.push_back(star);
                bodies.push_back(star);
            }
        }
        else if (type == "planet"){
            if(!(iss >> material))
            {
                throw runtime_error("Invalid data format for planet in file: " +fileName);
            }
            else{
                Planet planet(radius,mass,Vector(x,y,z),type,material);
                planets.push_back(planet);
                bodies.push_back(planet);
            }
        }
        else if (type == "moon"){
            if(!(iss >> planetID))
            {
                throw runtime_error("Invalid data format for moon in file: " +fileName);
            }
            else{
                Moon moon(radius,mass,Vector(x,y,z),type,planetID);
                moons.push_back(moon);
                bodies.push_back(moon);
            }
        }
        else if (type == "blackhole"){
            if(!(iss >> gravityuMultiplier))
            {
                throw runtime_error("Invalid data format for BlackHole in file: " +fileName);
            }
            else{
                BlackHole blackhole(radius,mass,Vector(x,y,z),type,gravityuMultiplier);
                blackholes.push_back(blackhole);
                bodies.push_back(blackhole);
            }
        }
    }
    file.close();

    return make_tuple(bodies , stars, planets, moons, blackholes);
}


