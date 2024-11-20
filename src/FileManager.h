#include <vector>
#include <string>
#include "Body.h"

class FileManager {
public:
    void loadConfig(const string& filePath, vector<Body>& bodies, double& timestep, double& gravityMultiplier, int& iterations);
};
