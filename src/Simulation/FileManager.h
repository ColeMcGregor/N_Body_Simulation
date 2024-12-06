#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <vector>
#include <string>


#include "vector.h"
#include "body.h"

struct Vector;
class Body;

class FileManager
{

    private:
        std::string fileName;    //name of the file we are reading
    public:
        FileManager(const std::string fileName);

  void loadConfig(const std::string &filePath,
                        std::vector<Body> &bodies,
                        double &timestep,
                        double &gravitationalMultiplier,
                        bool &stable,int &iterations,
                        int bodyCount[5]);

        void outputResults(const std::string &filePath,
                           const std::vector<Body> &bodies, 
                           double timeStep);
};

#endif
