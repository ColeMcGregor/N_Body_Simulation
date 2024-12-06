#include <vector>
#include <string>

#include "Body.h"
#include "Star.h"
#include "Planet.h"
#include "Moon.h"
#include "BlackHole.h"

#include "vector.h"

class FileManager
{

    private :
        string fileName;    //name of the file we are reading
    public:
        FileManager(const string fileName);

        void loadConfig( const string &filePath,
                        vector<Body> &bodies,
                        double &timestep,
                        double &gravitationalMultiplier,
                        bool &stable,int &iterations,
                        int bodyCount[5]);

        void outputResults(const string &filePath,
                           const vector<Body> &bodies, 
                           double timeStep);
};