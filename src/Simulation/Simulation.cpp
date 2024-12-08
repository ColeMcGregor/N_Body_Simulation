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

#include <iostream>
#include <vector>
#include <string>
#include <omp.h>
#include "vector.h"      // Include your Vector class header
#include "body.h"        // Include your Body class header
#include "FileManager.h" // Include your FileManager class header
#include "HeavenScapeBuilder.h"

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
    int bodyCount[5];               // information about the simulation bodies: 0: N, 1: NS, 2: NP, 3: NM, 4: NB, stored in the corresponding index of the array
    FileManager fileManager;        // file manager for the simulation

    Simulation(vector<Body> bodies, string outputFile, double timestep, double gravitationalMultiplier, int iterations, int bodyCount[5])
        : bodies(bodies), outputFile(outputFile), gravitationalMultiplier(gravitationalMultiplier) {}

    Simulation(const string &inputFile, const string &outputFile)
        : inputFile(inputFile), outputFile(outputFile), fileManager(inputFile) {
            // load the configuration file
            try {
                fileManager.loadConfig(inputFile, bodies, timestep, gravitationalMultiplier, iterations, bodyCount);
            } catch (const exception &e) {
                cout << "Error loading input file\n"
                        << e.what() << endl;
                exit(1);
        }
    }

    /**
     * @brief runs the simulation
     *
     * @details: will use OpenMP to parallelize the simulation, assigning a single thread to calculate the forces on each body,
     * and a single thread to output the results to the output file every 100 steps
     *
     * @param timeStep the timestep of the simulation
     * @param iterations the number of iterations of the simulation
     *
     */
    void run(double timeStep, int iterations)
    {
        // initialize total time variable
        double total_time = 0.0;

        // specifies how many bodies a thread will handle at any given time
        int chunk_size = bodies.size() / omp_get_max_threads();

        #pragma omp parallel
        {
            #pragma omp single
            {
                cout << "Using " << omp_get_num_threads() << " threads:" << endl
                        << endl;
            }

            // start timer for computation
            double start_comp_time = omp_get_wtime();

            for (int step = 0; step < iterations + 1; step++)
            {
                // divide the work among threads
                #pragma omp for schedule(dynamic, chunk_size)
                for (size_t i = 0; i < bodies.size(); i++)
                {
                    // cout << "Thread " << omp_get_thread_num() << "handling body " << i << endl;
                    //  calculate the total force on each body
                    Vector totalForce = bodies[i].sumForces(bodies);
                    bodies[i].applyForce(totalForce);
                    bodies[i].update(timeStep);
                }

// a single thread will output the results to the output file
                #pragma omp single
                {
                    if (step % 100000 == 0)
                    {
                        if (step == iterations)
                        {
                            // end timer for computation
                            double end_comp_time = omp_get_wtime();
                            cout << endl
                                    << step << " iterations reached, outputting to file..." << endl;
                            cout << endl
                                    << "Computation time: " << end_comp_time - start_comp_time << " seconds" << endl;

                            // start timer for outputting
                            double start_out_time = omp_get_wtime();
                            // outputs final results to destination file
                            fileManager.outputResults(outputFile, bodies, step);
                            // end timer for outputting
                            double end_out_time = omp_get_wtime();
                            cout << endl
                                    << "Destination: " << outputFile << endl;
                            cout << endl
                                    << "Outputting took " << end_out_time - start_out_time << " seconds" << endl;

                            // gather total time
                            total_time = (end_comp_time - start_comp_time) + (end_out_time - start_out_time);
                        }
                        else
                        {
                            cout << "[Simulation reached " << step << " iterations]" << endl;
                        }
                    }
                }
            }
        }
        cout << endl
                << "Total elapsed time: " << total_time << " seconds" << endl;
    }
};

int main(int argc, char *argv[])
{
    int option;
    bool isValid = false;
    // let the user choose between generating our solar system or loading an existing input file
    cout << "Please select an option:\n";
    cout << "1. Generate our solar system.\n";
    cout << "2. Load existing input file.\n";
    cout << "Enter your choice: ";
    cin >> option;
    cout << endl;

    vector<Body> bodies;
    double timeStep;
    int iterations;

    switch (option) {
        case 1;
            // prompt user for timestep and iterations
            cout << "Timestep: ";
            cin << timeStep;
            cout << endl << "Iterations: ";
            cin << iterations;
            cout << endl;

            // pass the timestep and iterations to the simulation

            // generates bodies according to our solar system
            bodies.generatePresetBodies();

            // create the simulation
            cout << "Creating simulation..." << endl;
            Simulation sim(bodies, "../output.txt", 1.0);
            // run the simulation
            cout << "Running simulation..." << endl;
            sim.run(timeStep, iterations);
            break;
        case 2;
            // check for correct number of arguments
            if (argc > 2)
            {
                cerr << "Usage: <filename> (optional)" << endl;
            }
            // create the simulation
            Simulation sim(inputFile, "../output.txt");
            // run the simulation
            sim.run(sim.timestep, sim.iterations);
        default:
            cerr << "Invalid option! Please try again..." << endl;
            exit(1);
    }
    return 0;
}
