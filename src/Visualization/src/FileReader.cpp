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
tuple<int,vector<Body>, std::vector<Star>, std::vector<Planet>, std::vector<Moon>, std::vector<BlackHole> > FileReader::readBodies() {
    
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

    //black hole var
    double gravityuMultiplier;

    //planet var
    string material;

    //for moon
    int planetID;

    //for star
    double luminosity;   

    //gives you the line of the file   
    std::string line;
    //while loop for reading in the file
    while (std::getline(file, line)) {
        //calls the iss in from line, returns the line , string takes the prefix, and int is the value
        istringstream iss(line);
        string prefix;
        int value;

        //looks for "Timestep: int value or NB or NS or NP or NM or NBH: with int following them all"
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
        //if it isnt in the first few lines, then it willl look for <radius> <mass> <x><y><z> and type followed by a special
        //variable for what kind of body it is
        else if (!(iss >> radius >> mass >> x >> y >> z >> type)) {
            throw std::runtime_error("Invalid data format in file: " + fileName);
        }
        //if its a star, assign special to luminosity
        else if(type == "star"){
            if(!(iss >> luminosity))
            {
                throw runtime_error("Invalid data format for star in file: " +fileName);
            }
            //push star to the star vector and bodies vector
            else{
                Star star(radius,mass,Vector(x,y,z),type,luminosity);
                stars.push_back(star);
                bodies.push_back(star);
            }
        }
        //if its a planet, assign special to material
        else if (type == "planet"){
            if(!(iss >> material))
            {
                throw runtime_error("Invalid data format for planet in file: " +fileName);
            }
            //push to planet and body vectors
            else{
                Planet planet(radius,mass,Vector(x,y,z),type,material);
                planets.push_back(planet);
                bodies.push_back(planet);
            }
        }
        //if its a moon, assign planetID
        else if (type == "moon"){
            if(!(iss >> planetID))
            {
                throw runtime_error("Invalid data format for moon in file: " +fileName);
            }
            //push to moon and body
            else{
                Moon moon(radius,mass,Vector(x,y,z),type,planetID);
                moons.push_back(moon);
                bodies.push_back(moon);
            }
        }
        //if its a blachkole, apply to gravityMult
        else if (type == "blackhole"){
            if(!(iss >> gravityuMultiplier))
            {
                throw runtime_error("Invalid data format for BlackHole in file: " +fileName);
            }
            //push to black hole and bodies vectors
            else{
                BlackHole blackhole(radius,mass,Vector(x,y,z),type,gravityuMultiplier);
                blackholes.push_back(blackhole);
                bodies.push_back(blackhole);
            }
        }
    }
    file.close();
    //returns timestep, bodies, stars, planets, moons and blackholes
    return make_tuple(timestep, bodies , stars, planets, moons, blackholes);
}


