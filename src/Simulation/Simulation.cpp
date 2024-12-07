/**
 * @file Simulation.cpp
 * @brief This file contains the implementation of the Simulation class, which is used to run the simulation
 *
 * @algorithm: this simulation will take in an input file, parse the information, and create the bodies in the simulation
 * it will then assign openMP threads to handle each body, and each thread will handle the calculations for that body
 * each thread will also handle the output of that body to a file
 *
 * @author: Brandon Trama, Cole McGregor, Hawk Lindner
 * @requirements: FileManager class, which is used to parse the input file for the creation of bodies in the simulation, and the output of the bodies to a file
 * @dependencies: body.cpp, filemanager.cpp
 */

#include <vector>
#include <string>
#include <omp.h>
#include "vector.h"      // Include your Vector class header
#include "body.h"        // Include your Body class header
#include "FileManager.h" // Include your FileManager class header

using namespace std;

class Simulation
{
public:
    vector<Body> bodies;            // vector of bodies in the simulation
    string inputFile;               // input file for the simulation
    string outputFile;              // output file for the simulation
    double timestep;                // timestep of the simulation
    double gravitationalMultiplier; // gravity multiplier of the simulation
    int iterations;                 // number of iterations of the simulation
    bool stable;                    // flag to toggle bounds for stable interactions(if gravity multiplier is not 1.00, this is irrelevant)
    int bodyCount[5];               // information about the simulation bodies: 0: N, 1: NS, 2: NP, 3: NM, 4: NB, stored in the corresponding index of the array
    FileManager fileManager;        // file manager for the simulation

  Simulation(const string &inputFile, const string &outputFile)
    : inputFile(inputFile), outputFile(outputFile), fileManager(inputFile)
    {
        // load the configuration file
        try
        {
            fileManager.loadConfig(inputFile, bodies, timestep, gravitationalMultiplier, stable, iterations, bodyCount);
        }
        catch (const exception &e)
        {
            cout << "Error loading configuration file\n" << e.what() << endl;
            exit(1);
        }
    }

    /**
     * @brief runs the simulation
     *
     * @details: will use openMP to parallelize the simulation, assigning a single thread to calculate the forces on each body,
     * and a single thread to output the results to the output file every 100 steps
     *
     * @param timeStep the timestep of the simulation
     * @param iterations the number of iterations of the simulation
     *
     */
    void run(double timeStep, int iterations)
    {
#pragma omp parallel
        {
            for (int step = 0; step < iterations; step++)
            {
// divide the work among threads
#pragma omp for
                for (size_t i = 0; i < bodies.size(); i++)
                {
                    // reset force before each calculation
                    bodies[i].resetForce();
                    // calculate the total force on each body
                    Vector totalForce = bodies[i].sumForces(bodies);
                    // apply the force to the body
                    bodies[i].applyForce(totalForce);
                }

// synchronize threads before updating positions
#pragma omp barrier

// update the bodies' positions and velocities
#pragma omp for
                for (size_t i = 0; i < bodies.size(); i++)
                {
                    bodies[i].update(timeStep);
                }

// synchronize threads before outputting results
#pragma omp barrier

// a single thread will output the results to the output file
#pragma omp single
                {
                    fileManager.outputResults(outputFile, bodies, step);
                }
            }
        }
    }
};

int main(int argz, char *argv[])
{
    // check for correct number of arguments
    if (argc != 4) {
        cerr << "Usage: <numthreads> <filename> <timestep> <iterations>" << endl;
    }

    // set the number of threads
    const int numThreads = atoi(argv[1]);

    // set the input file
    const string inputFile = "../" + argv[2];                                           // "../" is the specific path to the current input file, can be removed depending on where the input file is located
    // set the timestep and iterations
    const double timestep = atof(argv[3]);
    const int iterations = atoi(argv[4]);

    // set the output file
    const string outputFile = "output.txt";

    // create the simulation
    Simulation sim(inputFile, outputFile);
    //initiateHeavenscape(sim.bodies, sim.bodyCount);
    // set number of threads (default is number of cores)
    omp_set_num_threads(numThreads);
    // run the simulation
    sim.run(timestep, iterations);

    return 0;
}
