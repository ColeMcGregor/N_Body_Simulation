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
#include "vector.h"

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

    string line;        //line of the file
    int count = 0; //counting to get to the first line
    //while loop for reading in the file  

    int currid;
double radius, x, y, z;
istringstream iss(line);
string prefix;
int value;
string type;

// Parse lines with radius, mass, position (x, y, z), and type
int id;
while (std::getline(file, line)) {
    // Skip metadata lines (Timestep, N, etc.)
    if (count < 4) {
        count++;
        cout << line << endl;
        continue;
    }

    // Clear the stringstream and initialize it with the line content
    iss.clear();
    iss.str(line);

    // Parse the line for id, type, radius
    if (iss >> id >> type >> radius) {
        currid = id;
        // Create a new body object for each new ID with type and radius
        Body body(id, type, radius);
        
        // Now we start collecting positions for this body
        vector<Vector> currPos;  // Vector to hold positions for the current body
        while (std::getline(file, line)) {
            istringstream positionStream(line);

            // Check if the line contains position data (x, y, z)
            if (positionStream >> x >> y >> z) {
                Vector position(x, y, z);  // Create a Vector object with the parsed values
                currPos.push_back(position);  // Add the position to the positions vector
                cout << "Added position to body " << id << ": " << x << ", " << y << ", " << z << endl;
            } 
            // If we encounter another line with id type radius, stop and break out of the loop to process the next body
            else if (line.empty() || (line.find_first_of("0123456789") == 0 && line.find_first_of("star planet moon blackhole") != string::npos)) {
                break;
            }
        }

        // After all positions are collected, update the body with the positions
        body.updatePosition(currPos);
        localBodies.push_back(body);  // Add the body to the list of bodies
        cout << "Finished adding positions for body " << id << endl;
    }
}


            // } else if (type == "planet") {
            //     // Create and push the Planet object
            //     Planet planet(radius, Vector(x, y, z), type);
            //     localPlanets.push_back(planet);
            //     localBodies.push_back(planet);
            // } else if (type == "moon") {
            //     // Create and push the Moon object
            //     Moon moon(radius, Vector(x, y, z), type);
            //     localMoons.push_back(moon);
            //     localBodies.push_back(moon);
            // } else if (type == "blackhole") {
            //     // Create and push the BlackHole object
            //     BlackHole blackhole(radius, Vector(x, y, z), type);
            //     localBH.push_back(blackhole);
            //     localBodies.push_back(blackhole);
            // }
          // Skip to next line after finding a planet info (possibly for later logic)
        
        // else if (iss >> x >> y >> z) {
        //     if (type == "star") {
        //         // Create and push the Star object
        //         Star star(radius, Vector(x, y, z), type);
        //         localStars.push_back(star);
        //         localBodies.push_back(star);
        //     } else if (type == "planet") {
        //         // Create and push the Planet object
        //         Planet planet(radius, Vector(x, y, z), type);
        //         localPlanets.push_back(planet);
        //         localBodies.push_back(planet);
        //     } else if (type == "moon") {
        //         // Create and push the Moon object
        //         Moon moon(radius, Vector(x, y, z), type);
        //         localMoons.push_back(moon);
        //         localBodies.push_back(moon);
        //     } else if (type == "blackhole") {
        //         // Create and push the BlackHole object
        //         BlackHole blackhole(radius, Vector(x, y, z), type);
        //         localBH.push_back(blackhole);
        //         localBodies.push_back(blackhole);
        //     }
        // }
//     }
//     }
    file.close();
    //returns timestep, localBodies, localStars, planets, localMoons and localBH
    return make_tuple(localBodies , localStars, localPlanets, localMoons, localBH);
}

int main() {
    FileReader reader("output.txt");
    int timestep = reader.readTimeStep();
    auto[localBodies, localStars, localPlanets, localMoons, localBH] = reader.readBodies();
    cout << "Local Bodies size: " << localBodies.size() << endl;

    // Loop through each body and print its positions
    for (int i = 0; i < localBodies.size(); i++) {
        cout << "Body ID: " << localBodies[i].getID() << endl;
        vector<Vector> pos = localBodies[i].getPosition();
        for (int j = 0; j < pos.size(); j++) {
            // Output the body ID and the position (x, y, z) for each position
            cout << "Position " << j + 1 << ": " << pos[j].x << " " << pos[j].y << " " << pos[j].z << endl;
        }
    }

    return 0;
}


