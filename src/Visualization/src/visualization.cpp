#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <random>       // Assigning colors

#include <fstream>
#include <sstream>
#include <stdexcept>


#include "Body.h" // Assuming Body.cpp and Body.h define the Body class
#include "vector.h" // Assuming Vector is defined here

#include <algorithm>
using namespace std;

/*
    g++ -std=c++11 -o vis visualization.cpp FileReader.cpp Body.cpp Star.cpp Planet.cpp Moon.cpp BlackHole.cpp vector.cpp -framework OpenGL -framework GLUT -I/usr/local/include
*/


void parseInputFile(const string& fileName, int& timestep, int& bodyCount,
                    vector<Body>& bodies) {
    ifstream file(fileName);

    if (!file.is_open()) {
        throw runtime_error("Unable to open file: " + fileName);
    }

    string line;

    // Parse the timestep
    if (getline(file, line)) {
        istringstream iss(line);
        string label;
        iss >> label >> timestep;
        if (label != "Timestep:") {
            throw runtime_error("Invalid format for Timestep");
        }
    }
    // Parse the number of bodies
    if (getline(file, line)) {
        istringstream iss(line);
        string label;
        iss >> label >> bodyCount;
        if (label != "N:") {
            throw runtime_error("Invalid format for body count");
        }
    }

    // Parse body details
    do {
        istringstream iss(line);
        int id;
        string type;
        double radius;
        iss >> type >> id >> radius;
            
        
        Body body(type, id, radius);

        vector<Vector> trajectories;

        // Read trajectory lines
        while (getline(file, line)) {
            if (line.empty() || line.find_first_not_of("0123456789.,- ") != string::npos) {
                break;  // Stop parsing trajectories when a non-position line is found
            }

            istringstream posStream(line);
            double x, y, z;
            char comma;

            if (posStream >> x >> comma >> y >> comma >> z) {
                trajectories.emplace_back(Vector(x, y, z));  // Use the Vector class
            } else {
                throw runtime_error("Invalid trajectory format");
            }
        }

            body.updatePosition(trajectories);  // Update the body with parsed trajectories
            bodies.push_back(body);  // Add the body to the vector
        
        } while (!file.eof());

        file.close();
    }
int posx,posy,posz = 10;
// Function to set up the display
void display() {
    // Set the background color to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // RGBA: black background
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer
    glLoadIdentity();

    gluLookAt   (posx,posy,posz,
                0.0,0.0,0.0,
                0.0,1.0,0.0);

    for(int i = 1; i < bodies.size(); i++){

    }
    // Swap buffers (display the rendered image)
    glutSwapBuffers();
}

// Function to set up the OpenGL environment
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Black background color
    glLoadIdentity();
}
vector<Body> bodies;
int main(int argc, char** argv) {
    try {
        string fileName = "output.txt";
        int timestep = 0;
        int bodyCount = 0;


        parseInputFile(fileName, timestep, bodyCount, bodies);

        // Output parsed data for verification
        cout << "Timestep: " << timestep << endl;
        cout << "Number of Bodies: " << bodyCount << endl;


        cout << "\nBodies and Trajectories:\n";
        cout<<bodies.size()<<endl;
        for (const auto& body : bodies) {
            cout << "Body ID: " << body.getID() << ", Type: " << body.getType() << ", Radius: " << body.getRadius() << endl;
            //cout << "Trajectories:\n";
            // for (const auto& trajectory : body.getPosition()) {
            //     cout<< trajectory.x<<" "<<trajectory.y<<" "<<trajectory.z << endl;  // Assuming `Vector` overloads `operator<<`
            // }
            cout << endl;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    // Initialize GLUT
    glutInit(&argc, argv);

    // Set up a single buffered window with RGB color model
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);  // Set window size to 800x600 pixels
    glutCreateWindow("N-Body Sim");  // Create the window with a title

    // Initialize OpenGL
    init();

    // Register the display function to render the scene
    glutDisplayFunc(display);

    // Enter the GLUT main loop (will keep the window open)
    glutMainLoop();


    return 0;
}
