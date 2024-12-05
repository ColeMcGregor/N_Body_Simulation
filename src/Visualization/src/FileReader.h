#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <vector>
#include "Body.h"

class FileReader {
public:
    // Constructor
    FileReader(const std::string& fileName);

    // Reads file and initializes bodies
    std::vector<Body> readBodies();

private:
    std::string fileName; // Name of the file to read
};

#endif