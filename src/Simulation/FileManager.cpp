/**
 * This file contains the implementation of the FileManager class, which is used to parse the input file for the creation of bodies in the simulation
 * this is a stateless class, meaning that it does not maintain any internal state between calls to the loadConfig function
 * it is used to parse the input file and create the bodies in the simulation, and to output the bodies to a file
 * I AM A TOOL, NOT AN OBJECT
 *
 * @requirements: input.txt file, must be in the same directory as the executable, and correctly formatted
 * @output: output.txt file, will be in the same directory as the executable, used to simulate visualizations of the bodies in openGL
 *
 * @author: Cole McGregor, Hawk Lindner, Brandon Trama
 */

#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "body.h"
#include "vector.h"
using namespace std;

/**
 * @brief This function is used to load the configuration file and parse the information to create the bodies in the simulation
 * @param filePath: the path to the input file
 * @param bodies: the vector(datastructure that acts as a dynamic array) of bodies to be created
 * @param timestep: the timestep of the simulation
 * @param iterations: the number of iterations of the simulation
 * @param bodyCount: an array of integers that store the number of bodies of each type
 */
FileManager::FileManager(const string fileName) : fileName(fileName) {}
void FileManager::loadConfig(
    const string &filePath,
    vector<Body> &bodies,
    double &timestep,
    double &gravitationalMultiplier,
    int &iterations,
    int bodyCount[5])
{
    // Open the input file
    ifstream file(filePath);
    if (!file.is_open())
    {
        throw runtime_error("Unable to open file: " + filePath);
    }

    string line;

    // Parse the file line by line
    while (getline(file, line))
    {
        stringstream StringFileReader(line);
        string keyword;
        StringFileReader >> keyword;

        if (keyword == "Timestep")
        {
            StringFileReader >> timestep;
        }
        else if (keyword == "Iterations")
        {
            StringFileReader >> iterations;
        }
        else if (keyword == "N")
        {
            StringFileReader >> bodyCount[0]; // Total number of bodies
        }
        else if (keyword == "NS")
        {
            StringFileReader >> bodyCount[1]; // Number of stars
        }
        else if (keyword == "NP")
        {
            StringFileReader >> bodyCount[2]; // Number of planets
        }
        else if (keyword == "NM")
        {
            StringFileReader >> bodyCount[3]; // Number of moons
        }
        else if (keyword == "NB")
        {
            StringFileReader >> bodyCount[4]; // Number of blackholes
        }
        else if (keyword == "gravitationalMultiplier")
        {
            StringFileReader >> gravitationalMultiplier;
        }
        else if (keyword == "body")
        {
            // Parse body information
            int id;
            StringFileReader >> id;

            Vector position, velocity, accel, net_force = Vector(0, 0, 0);
            double mass, radius = 0.0;
            string type = "";
            vector<int> children;
            vector<Vector> trajectory;

            // Read subsequent lines for body details
            while (getline(file, line) && !line.empty())
            {                                            // while there is a line in the file and the line is not empty, stop reading for a body at a new line
                stringstream BodyStringFileReader(line); // create a stringstream object to read the line
                string attribute;                        // create a string to store the attribute of the body
                BodyStringFileReader >> attribute;       // read the attribute of the body and put it in the string

                if (attribute == "children")
                { // if the attribute is children
                    int childId;
                    while (BodyStringFileReader >> childId)
                    {                                // while there is an integer to read
                        children.push_back(childId); // add the integer to the children vector
                    }
                }
                else if (attribute == "position")
                {                                                                   // if the attribute is position
                    BodyStringFileReader >> position.x >> position.y >> position.z; // read the position of the body and put it in the vector
                }
                else if (attribute == "velocity")
                {                                                                   // if the attribute is velocity
                    BodyStringFileReader >> velocity.x >> velocity.y >> velocity.z; // read the velocity of the body and put it in the vector
                }
                else if (attribute == "mass")
                {                                 // if the attribute is mass
                    BodyStringFileReader >> mass; // read the mass of the body and put it in the double
                }
                else if (attribute == "radius")
                {                                   // if the attribute is radius
                    BodyStringFileReader >> radius; // read the radius of the body and put it in the double
                }
                else if (attribute == "star" || attribute == "planet" || attribute == "moon" || attribute == "blackhole")
                {
                    type = attribute; // if the attribute is a type of body, set the type of the body to the attribute
                }
            }

            // Create and store the body, using the constructor of the Body class
            bodies.emplace_back(position, velocity, accel, net_force, mass, radius, gravitationalMultiplier, type, children, trajectory); // create the body and add it to the vector
            // bodies.back().childrenIndices = children; // Assign parsed children indices for each body
        }
    }

    file.close(); // close the file
}

/**
 * @brief This function is used to output the locations of the bodies as the simulation runs, allowing for visualizations of the simulation
 * @param filePath: the path to the output file
 */
void FileManager::outputResults(const string &filePath, const vector<Body> &bodies, double timeStep)
{
    // open the file
    ofstream file(filePath);
    // check if the file is open
    if (!file.is_open())
    {
        throw runtime_error("Unable to open file: " + filePath);
    }
    // output the locations of the bodies to the file as the simulation runs(sets of "NValue x,y,z" coordinates)

    /**
     * below will output the timestep of the simulation
     * example:
     * Timestep: 1
     */

    file << "Timestep: " << timeStep << endl; // Timestep: 1

    /**
     * below will output the number of bodies in the simulation
     * example:
     * N: 10
     */
    file << "N: " << bodies.size() << endl; // N: 10

    /**
     * below will make a block of parent child relationships, in order of parent index followed by child indices
     * example:
     * 0 1 2 3
     * 1 4 5
     * 2 6 7
     * 3 8 9
     */
    for (size_t i = 0; i < bodies.size(); i++)
    {
        if (bodies[i].childrenIndices.size() > 0)
        {                     // if the body has children
            file << i << " "; // output the index of the body
            for (size_t j = 0; j < bodies[i].childrenIndices.size(); j++)
            { // output the indices of the children of the body
                file << bodies[i].childrenIndices[j] << " ";
            }
            file << endl;
        }
    }

/**
 * below will make a block of body number, body type, body radius, and trajectory for each body
 * example:
 * 0 star 100
 * 0.002 0.123 0.456
 * 0.003 0.124 0.457
 * 0.004 0.125 0.458
 * ...
 */

vector<string> thread_outputs(omp_get_max_threads());

// output block style trajectories
    #pragma omp parallel
    {   
        // output block style trajectories
        std::ostringstream local_stream;

        #pragma omp for schedule(static)
        for (size_t i = 0; i < bodies.size(); i++)
        { // for each body
            // body number, body type, body radius
            local_stream << bodies[i].type << " " << i << " " << bodies[i].radius << endl; // output the body number, type, and radius
            for (size_t j = 0; j < bodies[i].trajectory.size(); j++)
            {   
                // output the trajectory of the body
                local_stream << bodies[i].trajectory[j]; // Using the overloaded << operator
            }
        }
        thread_outputs[omp_get_thread_num()] = local_stream.str();
    }   
    // output the trajectories of the bodies to the file sequentially
    for (const string& thread_output : thread_outputs)
    {
        file << thread_output << endl;
    }

// output the locations of the bodies to the file
file.close();
}