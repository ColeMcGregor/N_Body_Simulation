#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <vector>
#include <tuple>
#include "vector.h"
#include "Body.h"
#include "Star.h"
#include "Planet.h"
#include "Moon.h"
#include "BlackHole.h"


class FileReader {
public:
    // Constructor
    FileReader(const std::string& fileName);

    // Reads file and initializes bodies
    std::tuple<std::vector<Body>, std::vector<Star>, std::vector<Planet>, std::vector<Moon>, std::vector<BlackHole> > readBodies();



private:
    std::string fileName; // Name of the file to read
};

#endif