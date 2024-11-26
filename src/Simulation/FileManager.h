#include <vector>
#include <string>
#include "body.h"
#include "vector.h"

class FileManager {
public:
    void loadConfig(const std::string& filePath, std::vector<Body>& bodies, double& timestep, double& gravityMultiplier, int& iterations);
    void FileManager::outputResults(const std::string& filePath, const std::vector<Body>& bodies, double timestep);
};
