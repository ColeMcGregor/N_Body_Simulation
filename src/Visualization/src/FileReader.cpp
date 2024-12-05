#include "FileReader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

FileReader::FileReader(const std::string& fileName) : fileName(fileName) {}

std::vector<Body> FileReader::readBodies() {
    std::vector<Body> bodies;
    std::ifstream file(fileName);

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + fileName);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        double mass, x, y, z, vx, vy, vz;
        
        if (!(iss >> mass >> x >> y >> z >> vx >> vy >> vz)) {
            throw std::runtime_error("Invalid data format in file: " + fileName);
        }

        // Create a Body object
        // Body body(mass, Vector(x, y, z), Vector(vx, vy, vz));
        // bodies.push_back(body);
    }

    file.close();
    return bodies;
}
