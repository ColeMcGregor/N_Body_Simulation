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
        : inputFile(inputFile), outputFile(outputFile)
    {
        // load the configuration file
        try
        {
            fileManager.loadConfig(inputFile, bodies, timestep, gravitationalMultiplier, stable, iterations, bodyCount);
        }
        catch (const exception &e)
        {
            cout << "Error loading configuration file" << e.what() << endl;
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
     * @param numSteps the number of steps to run the simulation
     *
     */
    void run(double timeStep, int numSteps)
    {
#pragma omp parallel
        {
            for (int step = 0; step < numSteps; step++)
            {
// divide the work among threads
#pragma omp for
                for (int i = 0; i < bodies.size(); i++)
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
                for (int i = 0; i < bodies.size(); i++)
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

/**
 * @brief generates a unique radius for a body
 * @param minRadius the minimum radius of the body
 * @param maxRadius the maximum radius of the body
 * @param usedRadii the vector of radii that have already been used
 * @return the unique radius
 */

double generateUniqueRadius(double minRadius, double maxRadius, const std::vector<double> &usedRadii)
{
    double radius;
    bool unique;
    do
    {
        radius = minRadius + (maxRadius - minRadius) * ((double)rand() / RAND_MAX); // Random within bounds
        unique = true;                                                              // flag to check if the radius is unique
        for (double r : usedRadii)
        { // check if the radius is unique
            if (fabs(radius - r) < 1.0e6)
            {                   // avoid radii too close to others
                unique = false; // set the flag to false if the radius is not unique
                break;          // break out of the loop if the radius is not unique
            }
        }
    } while (!unique);
    return radius;
}

/**
 * @brief calculates the orbital position of a body, depending on the parent body's position and the orbital radius
 * @param parentPos the position of the parent body
 * @param orbitalRadius the orbital radius of the body(this is the distance from the parent body you want to be away from)
 * @return the orbital position of the body
 */
Vector calculateOrbitalPosition(const Vector &parentPos, double orbitalRadius)
{
    double theta = ((double)rand() / RAND_MAX) * 2.0 * M_PI; // Azimuthal angle [0, 2π]
    double phi = ((double)rand() / RAND_MAX) * M_PI;         // Inclination angle [0, π]

    double x = orbitalRadius * sin(phi) * cos(theta); // x position
    double y = orbitalRadius * sin(phi) * sin(theta); // y position
    double z = orbitalRadius * cos(phi);              // z position

    return Vector(parentPos.x + x, parentPos.y + y, parentPos.z + z); // return the orbital position of the body
}

/**
 * @brief calculates the orbital velocity of a body, depending on the parent body's position and the orbital radius
 * @param parentPos the position of the parent body
 * @param childPos the position of the child body
 * @param parentMass the mass of the parent body
 * @param gravitationalMultiplier the gravitational multiplier of the simulation
 * @return the orbital velocity of the body
 */
Vector calculateOrbitalVelocity(const Vector &parentPos, const Vector &childPos, double parentMass, double gravitationalMultiplier)
{
    const double G = 6.67430e-11 * gravitationalMultiplier;

    // Calculate the distance vector and magnitude
    Vector r = childPos - parentPos;
    double distance = r.magnitude();

    // Orbital speed
    double speed = sqrt((G * parentMass) / distance);

    // Calculate a perpendicular velocity vector
    Vector unitR = r / distance;           // Unit vector of position
    Vector velocity(-unitR.y, unitR.x, 0); // Perpendicular in XY-plane

    // Normalize and scale to orbital speed
    velocity = velocity * speed;
    // return the orbital velocity of the body
    return velocity;
}
/**
 * @brief this algorithm is used to set the correct positions, velocities, and orbital velocities of the bodies
 * @details:
 * @param bodies the vector of bodies in the simulation
 * @param bodyCount the array of body counts ([0]: N, [1]: NS, [2]: NP, [3]: NM, [4]: NB)
 */
void initiateHeavenscape(vector<Body> &bodies, int bodyCount[5])
{
    /*
     * Determine the hierarchical structure and spatial arrangement of celestial bodies in the simulation:
     *
     * 1. Identify if black holes are present: (bodyCount[4] > 0)
     *    - If present, the black hole is(are) the heaviest body(ies) and serves as the center of the simulation.
     *    - Analyze the black hole's children (stars, planets, or both) and assign them appropriately.
     *    - the closest black hole to us is 1560 light years away, or 1.4759e+19 meters, which can be used to average the distance between black holes, if more than one
     *
     * 2. If no black holes are present: (bodyCount[4] == 0)
     *    - Designate the most massive body as the center of the simulation.
     *
     * 3. Arrange stars: (bodyCount[1] > 0)
     *    - Determine the number of stars and ensure sufficient spacing between them to accommodate their planetary systems.
     *    - Place stars in stable orbits around the black hole or the center of the simulation.
     *    - If multiple stars exist, ensure they are spaced apart to allow for planetary systems.
     *    - the closest star from sol is Alpha Centauri at 4.37 light years away, or 3.8 x 10^16 meters, which can be used to average the distance between stars
     *
     * 4. Assign planets: (bodyCount[2] > 0)
     *    - Determine the number of planets and assign each to the correct parent (star or black hole).
     *    - Ensure sufficient spacing between planets to allow for moon systems.
     *    - the closest planet to sol is Mercury at 57 million kilometers, or 5.7 x 10^10 meters, which can be used to bind the distance from a planet to a star
     *    - the shortest distance between any two planets is 0.28 AU, or 4.1887e+10, which can be used to average the minimum distance between planets
     *    - the longest distance between two planets in our solar system is 13.2 AU, or 1.9747e+12 meters, which can be used to average the maximum distance between planets around a star
     *
     * 5. Handle moons: (bodyCount[3] > 0)
     *    - Identify all moons and assign them to their appropriate parent planets.
     *    - Position moons after planets, ensuring their orbital radii are correctly assigned.
     *    - the closest moon to any planet in our solar system is phobos to mars, at an astonishly low 6000 km, or 6 x 10^6 meters, which can be used to average the minimum distance between moons and planets
     *    - the moon farthest from its host body is Neso from neptune, at 4.96 x 10^10 meters, which can be used to average the maximum distance between moons and planets
     *
     *  6. Special distribution cases (UNSTABLE INTERACTIONS)
     *    - The above bounds for the distances between bodies can be toggled off to create unstable interactions, this will result in the bodies spiraling into eachother, or out of the system
     *    - Additionally, the gravitional multiplier when not set at 1 will create unstable interactions, as the force of gravity will be too great or too small, causing the bodies to behave erratically
     *        - it should be noted that the bounds do not work at all for stable orbits when the gravitational multiplier is not 1.00
     *    - the stable flag in the input file can be toggled to allow for stable orbits when the gravitational multiplier is set to 1.00, otherwise it is irrelevant
     */
}

int main()
{
    const string inputFile = "input.txt";
    const string outputFile = "output.txt";

    // create the simulation
    Simulation sim(inputFile, outputFile);
    initiateHeavenscape(sim.bodies, sim.bodyCount);
    // set number of threads (default is number of cores)
    omp_set_num_threads(omp_get_max_threads());
    // run the simulation
    sim.run(0.01, 1000);

    return 0;
}
