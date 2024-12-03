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
#include "vector.h" // Include your Vector class header
#include "body.h" // Include your Body class header
#include "FileManager.h" // Include your FileManager class header

using namespace std;

class Simulation {
    public:
        vector<Body> bodies; // vector of bodies in the simulation
        string inputFile; // input file for the simulation
        string outputFile; // output file for the simulation
        double timestep; // timestep of the simulation
        double gravityMultiplier; // gravity multiplier of the simulation
        int iterations; // number of iterations of the simulation
        FileManager fileManager; // file manager for the simulation

        Simulation(const string& inputFile, const string& outputFile)
            : inputFile(inputFile), outputFile(outputFile) {
                // load the configuration file
                fileManager.loadConfig(inputFile, bodies, timestep, gravityMultiplier, iterations);
            }
        
        void run(double timeStep, int numSteps) {
            #pragma omp parallel
            {   
                for (int step = 0; step < numSteps; step++) {
                    // divide the work among threads
                    #pragma omp for
                    for (int i = 0; i < bodies.size(); i++) {
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
                    for (int i = 0; i < bodies.size(); i++) {
                        bodies[i].update(timeStep);
                    }

                    // synchronize threads before outputting results
                    #pragma omp barrier
                    
                    // a single thread will output the results to the output file every 100 steps
                    #pragma omp single
                    {
                        if (step % 100 == 0) {
                            fileManager.outputResults(outputFile, bodies, step);
                        }
                    }
                }
            }
        }
};

/**
 * @brief generates a unique radius for a body
 * @param minRadius the minimum radius of the body
 * @param maxRadius the maximum radius of the body
 * @param usedRadii the vector of radii that have already been used
 * @return the unique radius
 */

double generateUniqueRadius(double minRadius, double maxRadius, const std::vector<double>& usedRadii) {
    double radius;
    bool unique;
    do {
        radius = minRadius + (maxRadius - minRadius) * ((double)rand() / RAND_MAX); // Random within bounds
        unique = true;                                                              // flag to check if the radius is unique
        for (double r : usedRadii) {                                                // check if the radius is unique
            if (fabs(radius - r) < 1.0e6) {                                         // avoid radii too close to others
                unique = false;                                                     // set the flag to false if the radius is not unique
                break;                                                              // break out of the loop if the radius is not unique
            }
        }
    } while (!unique);
    return radius;
}

/**
 * @brief calculates the orbital position of a body, depending on the parent body's position and the orbital radius
 * @param parentPos the position of the parent body
 * @param orbitalRadius the orbital radius of the body
 * @return the orbital position of the body
 */
Vector calculateOrbitalPosition(const Vector& parentPos, double orbitalRadius) {
    double theta = ((double)rand() / RAND_MAX) * 2.0 * M_PI; // Azimuthal angle [0, 2π]
    double phi = ((double)rand() / RAND_MAX) * M_PI;         // Inclination angle [0, π]

    double x = orbitalRadius * sin(phi) * cos(theta);   // x position 
    double y = orbitalRadius * sin(phi) * sin(theta);   // y position
    double z = orbitalRadius * cos(phi);                // z position

    return Vector(parentPos.x + x, parentPos.y + y, parentPos.z + z);  // return the orbital position of the body
}


/**
 * @brief calculates the orbital velocity of a body, depending on the parent body's position and the orbital radius
 * @param parentPos the position of the parent body
 * @param childPos the position of the child body
 * @param parentMass the mass of the parent body
 * @param gravitationalMultiplier the gravitational multiplier of the simulation
 * @return the orbital velocity of the body
 */
Vector calculateOrbitalVelocity(const Vector& parentPos, const Vector& childPos, double parentMass, double gravitationalMultiplier) {
    const double G = 6.67430e-11 * gravitationalMultiplier;

    // Calculate the distance vector and magnitude
    Vector r = childPos - parentPos;
    double distance = r.magnitude();

    // Orbital speed
    double speed = sqrt(G * parentMass / distance);

    // Calculate a perpendicular velocity vector
    Vector unitR = r / distance; // Unit vector of position
    Vector velocity(-unitR.y, unitR.x, 0); // Perpendicular in XY-plane

    // Normalize and scale to orbital speed
    velocity = velocity * speed;

    return velocity;
}




int main() {
    const string inputFile = "input.txt";
    const string outputFile = "output.txt";

    // create the sim
    Simulation sim(inputFile, outputFile);
    // set number of threads (default is number of cores)
    omp_set_num_threads(omp_get_max_threads());
    // run the sim
    sim.run(0.01, 1000);

    return 0;
}
