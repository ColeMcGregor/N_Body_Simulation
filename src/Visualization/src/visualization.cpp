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


GLuint earthTexture, starTexture;

/*
    g++ -std=c++11 -o vis visualization.cpp FileReader.cpp Body.cpp Star.cpp Planet.cpp Moon.cpp BlackHole.cpp vector.cpp -framework OpenGL -framework GLUT -I/usr/local/include
*/
vector<Body>bodies;
float posx = 500.0;
float posy = 300.0;
float posz = 100.0;
const int TIMER_INTERVAL = 16; //~60FPS

float zoomFactor = 0.1f;  // Adjust to control zoom speed
float minZoom = 1.0f;     // Minimum distance from the center
float maxZoom = 1000.0f;  // Maximum distance from the center


void keyboard(unsigned char key, int x, int y) {
    // Compute the length of the camera's position vector (distance from origin)
    float distance = sqrt(posx * posx + posy * posy + posz * posz);

    // Unit vector components of the camera position
    float ux = posx / distance;
    float uy = posy / distance;
    float uz = posz / distance;

    switch (key) {
        case 'w':  // Zoom in
            distance -= distance * zoomFactor;  // Reduce distance
            if (distance < minZoom) distance = minZoom;
            break;
        case 's':  // Zoom out
            distance += distance * zoomFactor;  // Increase distance
            if (distance > maxZoom) distance = maxZoom;
            break;
        case 27:  // Escape key
            exit(0);  // Exit the program
            break;
    }

    // Update camera position based on the scaled unit vector
    posx = ux * distance;
    posy = uy * distance;
    posz = uz * distance;

    glutPostRedisplay();  // Redraw the scene
}


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


// Function to update the positions of all bodies
void updatePositions() {
    for (auto& body : bodies) {
        if (!body.getPosition().empty()) {
            // Update the current position index
            int currentIndex = body.getCurrIndex();
            currentIndex = (currentIndex + 1) % body.getPosition().size(); // Loop back to start
            body.setCurrIndex(currentIndex);
        }
    }
    glutPostRedisplay(); // Trigger a redraw
}

// Timer callback function
void timer(int value) {
    updatePositions(); // Update body positions
    glutTimerFunc(TIMER_INTERVAL, timer, 0); // Schedule next timer call
}
// Function to calculate the maximum distance of any body from the origin


float calculateMaxDistance() {
    float maxDist = 0.0f;
    
    // Check if bodies vector is populated
    if (bodies.empty()) {
        cout << "Error: bodies vector is empty!" << endl;
        return maxDist;  // If no bodies, return 0 distance
    }

    // Loop through each body and calculate the distance from the origin
    for (int i = 1; i < bodies.size(); i++) {
        vector<Vector> positions = bodies[i].getPosition();
        
        if (positions.empty()) {
            cout << "Error: Body " << bodies[i].getID() << " has no positions!" << endl;
            continue;  // Skip this body if no positions exist
        }

        // Get the first position (you can modify this if you want to use other positions)
        Vector pos = positions[0];

        // Calculate the distance from the origin (0, 0, 0)
        float dist = sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);  // Distance formula

        // Update the maximum distance if needed
        if (dist > maxDist) {
            maxDist = dist;
        }
    }

    return maxDist;
}

void setMaterialProperties(int id) {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess[1];

    if (id == 1) {
        // Gray with metallic highlights
        ambient[0] = 0.4f; ambient[1] = 0.4f; ambient[2] = 0.4f; ambient[3] = 1.0f;
        diffuse[0] = 0.8f; diffuse[1] = 0.8f; diffuse[2] = 0.7f; diffuse[3] = 1.0f;
        specular[0] = 0.9f; specular[1] = 0.9f; specular[2] = 0.9f; specular[3] = 1.0f;
        shininess[0] = 50.0f;
    } else if (id == 2) {
        // Yellowish-Brown with soft highlights
        ambient[0] = 0.6f; ambient[1] = 0.5f; ambient[2] = 0.2f; ambient[3] = 1.0f;
        diffuse[0] = 0.9f; diffuse[1] = 0.8f; diffuse[2] = 0.6f; diffuse[3] = 1.0f;
        specular[0] = 0.3f; specular[1] = 0.3f; specular[2] = 0.2f; specular[3] = 1.0f;
        shininess[0] = 25.0f;
    } else if (id == 3) {
        // Blue with vibrant specular highlights
        ambient[0] = 0.0f; ambient[1] = 0.2f; ambient[2] = 0.5f; ambient[3] = 1.0f;
        diffuse[0] = 0.0f; diffuse[1] = 0.4f; diffuse[2] = 1.0f; diffuse[3] = 1.0f;
        specular[0] = 0.6f; specular[1] = 0.6f; specular[2] = 0.6f; specular[3] = 1.0f;
        shininess[0] = 80.0f;
    } else if (id == 4) {
        // Red-Orange with a rocky appearance
        ambient[0] = 0.5f; ambient[1] = 0.1f; ambient[2] = 0.0f; ambient[3] = 1.0f;
        diffuse[0] = 1.0f; diffuse[1] = 0.3f; diffuse[2] = 0.0f; diffuse[3] = 1.0f;
        specular[0] = 0.2f; specular[1] = 0.1f; specular[2] = 0.0f; specular[3] = 1.0f;
        shininess[0] = 10.0f;
    } else if (id == 5) {
        // Brownish-Orange with subtle highlights
        ambient[0] = 0.4f; ambient[1] = 0.3f; ambient[2] = 0.2f; ambient[3] = 1.0f;
        diffuse[0] = 0.9f; diffuse[1] = 0.7f; diffuse[2] = 0.5f; diffuse[3] = 1.0f;
        specular[0] = 0.3f; specular[1] = 0.2f; specular[2] = 0.1f; specular[3] = 1.0f;
        shininess[0] = 15.0f;
    } else if (id == 6) {
        // Pale Yellow with a smooth finish
        ambient[0] = 0.6f; ambient[1] = 0.6f; ambient[2] = 0.3f; ambient[3] = 1.0f;
        diffuse[0] = 0.9f; diffuse[1] = 0.8f; diffuse[2] = 0.5f; diffuse[3] = 1.0f;
        specular[0] = 0.4f; specular[1] = 0.4f; specular[2] = 0.2f; specular[3] = 1.0f;
        shininess[0] = 20.0f;
    } else if (id == 7) {
        // Light Blue with soft highlights
        ambient[0] = 0.3f; ambient[1] = 0.6f; ambient[2] = 0.8f; ambient[3] = 1.0f;
        diffuse[0] = 0.5f; diffuse[1] = 0.8f; diffuse[2] = 1.0f; diffuse[3] = 1.0f;
        specular[0] = 0.3f; specular[1] = 0.5f; specular[2] = 0.6f; specular[3] = 1.0f;
        shininess[0] = 30.0f;
    } else if (id == 8) {
        // Deep Blue with reflective highlights
        ambient[0] = 0.0f; ambient[1] = 0.1f; ambient[2] = 0.4f; ambient[3] = 1.0f;
        diffuse[0] = 0.0f; diffuse[1] = 0.3f; diffuse[2] = 1.0f; diffuse[3] = 1.0f;
        specular[0] = 0.4f; specular[1] = 0.4f; specular[2] = 0.6f; specular[3] = 1.0f;
        shininess[0] = 40.0f;
    } else {
        // Default white
        ambient[0] = 0.2f; ambient[1] = 0.2f; ambient[2] = 0.2f; ambient[3] = 1.0f;
        diffuse[0] = 1.0f; diffuse[1] = 1.0f; diffuse[2] = 1.0f; diffuse[3] = 1.0f;
        specular[0] = 1.0f; specular[1] = 1.0f; specular[2] = 1.0f; specular[3] = 1.0f;
        shininess[0] = 50.0f;
    }

    

    // Apply the material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

void drawRings(float innerRadius, float outerRadius, int segments, float red, float green, float blue) {
    glPushMatrix();

    // Set vibrant ring colors
    glDisable(GL_LIGHTING);  // Disable lighting to emphasize color
    glColor3f(red, green, blue);  // Ring color

    // Draw the ring using a triangle strip
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;

        float xInner = innerRadius * cos(angle);
        float zInner = innerRadius * sin(angle);

        float xOuter = outerRadius * cos(angle);
        float zOuter = outerRadius * sin(angle);

        // Inner point
        glVertex3f(xInner, 0.0f, zInner);

        // Outer point
        glVertex3f(xOuter, 0.0f, zOuter);
    }
    glEnd();

    glEnable(GL_LIGHTING);  // Re-enable lighting for other objects
    glPopMatrix();
}


void drawRing(float radius, float red, float green, float blue) {
    glColor3f(red, green, blue);  // Set the color for the ring
    glBegin(GL_LINE_LOOP);
    int numSegments = 100;  // Number of segments to approximate the circle
    for (int i = 0; i < numSegments; i++) {
        float angle = 2.0f * M_PI * i / numSegments;  // Calculate angle for this segment
        float x = radius * cos(angle);               // X-coordinate
        float z = radius * sin(angle);               // Z-coordinate
        glVertex3f(x, 0.0f, z);                      // Vertex on the XZ-plane
    }
    glEnd();
}
// Function to set up the display
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear the color buffer
    glLoadIdentity();

    gluLookAt(posx, posy, posz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();

    float maxDist = calculateMaxDistance();
    float scaleFactor = 100.0f / maxDist;

    for (int i = 1; i < bodies.size(); i++) {
        Body body = bodies[i];
        string type = body.getType();
        float radius = body.getRadius();
        int currentIndex = body.getCurrIndex();
        Vector pos = body.getPosition()[currentIndex];

        float scaledRadius = radius * scaleFactor;

        // Draw orbit ring
        glColor3f(255.0f, 165.0f, 0.0f); // Gray color for the orbit
        glPushMatrix();
        float orbitRadius = sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z) * scaleFactor;
        glBegin(GL_LINE_LOOP);
        int numSegments = 100; // Number of segments for the circle
        for (int j = 0; j < numSegments; j++) {
            float theta = 2.0f * M_PI * float(j) / float(numSegments); // Current angle
            float x = orbitRadius * cos(theta);
            float z = orbitRadius * sin(theta);
            glVertex3f(x, 0.0f, z); // Orbit lies in the xz-plane
        }
        glEnd();
        glPopMatrix();

        // Draw body (planet or star)
        if (type == "star") {
            glDisable(GL_LIGHTING);
            glColor3f(1.0f, 1.0f, 0.0f);
            glPushMatrix();
            glTranslatef(pos.x * scaleFactor, pos.y * scaleFactor, pos.z * scaleFactor);
            glutSolidSphere(scaledRadius / 100, 100, 100);
            glPopMatrix();
            glEnable(GL_LIGHTING);
        }else if (type == "planet") {
            if (body.getID() == 2) {
            glColor3f(80.0f, 80.0f, 70.0f);  // Gray
        } else if (body.getID() == 3) {
            glColor3f(0.9f, 0.8f, 0.6f);  // Yellowish-Brown
        } else if (body.getID() == 4) {
            glColor3f(0.0f, 40.0f, 100.0f);  // Blue
        } else if (body.getID() == 5) {
            glColor3f(1.0f, 0.3f, 0.0f);  // Red-Orange
        } else if (body.getID() == 6) {
            glColor3f(0.9f, 0.7f, 0.5f);  // Brownish-Orange
        } else if (body.getID() == 7) {
            glColor3f(0.9f, 0.8f, 0.5f);  // Pale Yellow
        } else if (body.getID() == 8) {
            glColor3f(0.5f, 0.8f, 1.0f);  // Light Blue
        } else if (body.getID() == 9) {
            glColor3f(0.0f, 0.3f, 1.0f);  // Deep Blue
        } else {
            glColor3f(1.0f, 1.0f, 1.0f);  // Default white for unknown objects
        }
            glPushMatrix();
            setMaterialProperties(body.getID());
            glTranslatef(pos.x * scaleFactor, pos.y * scaleFactor, pos.z * scaleFactor);  // Apply scaling
            glutSolidSphere(scaledRadius, 100, 100);
            glPopMatrix();

            if(body.getID() == 6){
                glPushMatrix();
                glTranslatef(pos.x * scaleFactor, pos.y * scaleFactor, pos.z * scaleFactor);
               drawRings(scaledRadius * 1.2f, scaledRadius * 2.0f, 100, 0.8f, 0.8f, 0.5f);
                glPopMatrix();
            }
            
            // Draw orbit ring
            glPushMatrix();
            glTranslatef(0.0f, pos.y * scaleFactor, 0.0f);  // Center the ring around the star
            drawRing(pos.x * scaleFactor, 255.0f, 165.5f, 1.0f);  // Blue ring
            glPopMatrix();

            

        }
    }

    glPopMatrix();
    glutSwapBuffers();
}

/*reshaping for the window*/
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

void setupLighting(const Vector& starPosition) {
    glEnable(GL_LIGHTING);    // Enable lighting
    glEnable(GL_LIGHT0);      // Enable light source 0
    glEnable(GL_DEPTH_TEST);  // Enable depth testing for proper rendering

    // Light position
    GLfloat lightPos[] = { 
        static_cast<GLfloat>(starPosition.x), 
        static_cast<GLfloat>(starPosition.y), 
        static_cast<GLfloat>(starPosition.z), 
        1.0f  // Positional light
    };

    // Light properties
    GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };   // Minimal ambient light
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 0.8f, 1.0f };   // Soft white-yellow light
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // Specular highlights

    // Apply light properties
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
}

// Function to set up the OpenGL environment
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Black background color
    glEnable(GL_DEPTH_TEST);  // Enable depth testing for 3D rendering
    glLoadIdentity();
}
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
    #ifdef _WIN32
    sleep(10000);
    #endif

    // Initialize GLUT
    glutInit(&argc, argv);

    // Set up a single buffered window with RGB color model
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);  // Set window size to 800x600 pixels
    glutCreateWindow("N-Body Sim");  // Create the window with a title

    // Initialize OpenGL
    init();
    setupLighting(bodies[1].getPosition()[Body::currIndex]);
    // Register the display function to render the scene
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);                                               // Register the reshape callback
    glutKeyboardFunc(keyboard);   
    // Enter the GLUT main loop (will keep the window open)

    glutTimerFunc(TIMER_INTERVAL, timer, 0);
    glutMainLoop();


    return 0;
}
