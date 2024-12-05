#include <GL/glut.h>
#include <iostream>
#include <random>
//#include "body.h" //for vector class
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

/*
    This is my program that works in OpenGL, GLU, and creating a solar system representation for Parallel final project

        clang++ -std=c++11 -Iinclude -o  app src/main.cpp src/glad.c -lglfw -ldl -framework OpenGL -framework GLUT -framework GLUT -DGL_SILENCE_DEPRECATION;
    @author: Hawk Lindner, ChatGPT 4.0
*/


/*
    ToDo:
        create struct for stars, planets, moons
        create shaders for the planets from where the stars are 
        Make light sources come from every star
*/

/*
    In the works
        Camera can be interacted with (zoom in and out, move around, mouse click and drag)
*/

/*
    Structs for the program
*/
struct Vector{
    float x,y,z;
    Vector(float x_ = 0.0, float y_ = 0.0, float z_ = 0.0) : x(x_), y(y_), z(z_) {}
};

struct Camera{
    float eyex,eyey,eyez;
    float radius;       
    float yaw;
    float pitch;
    Vector focusPoint;

    Camera(float r = 20.0f, float y = 0.0f, float p = 20.0f, Vector focus = Vector(0.0, 0.0, 0.0))
        : radius(r), yaw(y), pitch(p), focusPoint(focus) {}

    void applyCameraView() {
        // Convert yaw and pitch from degrees to radians
        constexpr float DEG_TO_RAD = 3.14159265f / 180.0f;
        float radYaw = yaw * DEG_TO_RAD;
        float radPitch = pitch * DEG_TO_RAD;

        // Calculate camera position in Cartesian coordinates
        eyex = focusPoint.x + radius * cos(radPitch) * sin(radYaw);
        eyey = focusPoint.y + radius * sin(radPitch);
        eyez = focusPoint.z + radius * cos(radPitch) * cos(radYaw);

        // Apply the gluLookAt function to position the camera
        gluLookAt(eyex, eyey, eyez,    // Camera position
                  focusPoint.x, focusPoint.y, focusPoint.z,  // Look-at point
                  0.0, 1.0, 0.0);       // Up direction
    }
    // Method to handle mouse input for adjusting yaw and pitch
    void handleMouseMovement(float deltaX, float deltaY) {
        // Sensitivity factor to control the camera movement speed
        float sensitivity = 0.1f;

        // Update yaw and pitch based on mouse movement
        yaw += deltaX * sensitivity;
        pitch -= deltaY * sensitivity;

        // Clamp pitch to avoid flipping the camera
        if (pitch > 89.0f) pitch = 89.0f; 
        if (pitch < -89.0f) pitch = -89.0f;
    }
    // Method to handle zoom input for adjusting camera radius
    void handleZoom(float zoomAmount) {
        radius -= zoomAmount;
        // Clamp radius to avoid getting too close or too far
        if (radius < 5.0f) radius = 5.0f;
        if (radius > 100.0f) radius = 100.0f;
    }

};

/*Struct to define a sphere*/
typedef struct sphere{
    float x,y,z;            //position
    float radius;           //radius of sphere
    // float rotationalSpeed;
    // float rotationAngle;
    float color[3];//rgb color
}sphere;

/*struct for a star object*/
typedef struct star{
    float x,y,z;                //position
    float radius;               //radius of star
    float rotationalSpeed;      //rotational speed of the star
    float mass;                 //stars mass
    float luminosity;           //stars luminosity
    float color[3];//rgb color
//    Vector velocity;            //vector for the velocity
//    Vector netForce;            //net force of the star
//    Vector acceleration;        //stars acceleration
    std::string name;           //name of the star

}star;

/*struct for a planet object*/
typedef struct planet{
    float x,y,z;                //position
    float radius;               //radius of body
    float rotationalSpeed;      //rotational speed of body
    float mass;                 //mass of planet
//    Vector netForce;            //Net force currently acting on the planet (used to calculate acceleration)
//    Vector velocity;            //speed to track planet
    float rotationalPeriod;     //time to rotate 365 degrees
    float orbitalPeriod;        //time to orbit the planets star
    float color[3];//rgb color
    int planet_id;              //id for moons
    std::string name;           //name of planet
}planet;

/*struct for a moon object*/
typedef struct moon{
    float x,y,z;                //position
    float radius;               //radius of body
    float rotationalSpeed;      //rotational speed of moon
    float mass;                 //mass of the moon
    float orbitalPeriod;        //time it takes to orbit planet 
    float color[3];//rgb color
//    Vector netForce;            //net force currently acting on planet (used to calculate acceleration)
//    Vector velocity;            //speed of the moon
    int planet_parent_id;       //host planet id
    std::string name;           //name of the moon
}moon;

/*
    Global variables for the program
*/
/*for the camer=*/
float eyex = 1.0, eyey = 10.0, eyez = 50.0;
/*This will speed up or slow down the speed of orbit*/
float timescale = 1.0f;
/*used for updateCameraPosition function*/
float angle = 0.0;
/*Earth and moon orbiting angles*/
float earthOrbitingAngle = 0.0f;
float moonOrbitAngle = 0.0f;
/*Will be the timestamp for zooming*/
float zoomFactor = 1.0f;

float cameraRadius = 20.0f;  // Distance from the focus point
float cameraYaw = 0.0f;      // Horizontal angle in degrees
float cameraPitch = 20.0f;   // Vertical angle in degrees
const Vector focusPoint =( 0.0, 0.0, 0.0); // The point to focus on (center of scene)

float lastMouseX, lastMouseY;
bool isDragging = false;


//defines how many spheres
//This is for the array of spheres
#define NUM_SPHERES 3
sphere spheres[NUM_SPHERES];

/*seeing if the mouse is being dragged*/
void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        } else if (state == GLUT_UP) {
            isDragging = false;
        }
    }
}

/*function to see if the mouse is dragging*/
void mouseMotion(int x, int y) {
    if (isDragging) {
        // Calculate the difference in mouse position
        float deltaX = x - lastMouseX;
        float deltaY = y - lastMouseY;

        // Update yaw and pitch based on mouse movement (adjust sensitivity if needed)
        float sensitivity = 0.1f;
        cameraYaw += deltaX * sensitivity;
        cameraPitch -= deltaY * sensitivity;

        // Clamp the pitch to prevent the camera from flipping over
        if (cameraPitch > 89.0f) cameraPitch = 89.0f;
        if (cameraPitch < -89.0f) cameraPitch = -89.0f;

        // Store the current mouse position for the next event
        lastMouseX = x;
        lastMouseY = y;

        // Redraw the scene with the new camera angles
        glutPostRedisplay();
    }
}

/*
    This takes in the input from the keyboard and does stuff
*/
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': eyez -= 0.1f; //need to be fixed
            if(eyez < 1.0){
                eyez = 1.0;
            } break; // Move closer
        case 's': eyez += 0.1f; 
              break; // Move farther  //needs to be fixed
        case  27: exit(0); break;   //esc key           //works
        case '+': zoomFactor += 0.1f;                   //needs to be fixed
            std::cout <<"Zooming in" << ::std::endl; break;
        case '-': zoomFactor -= 0.1f;                   //needs to be fixed
            std::cout <<"Zooming out" << ::std::endl; break;
        
    }
    glutPostRedisplay(); // Redraw the scene
}

/*
    sets up the light for the sun
*/  
void setupLighting() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING); // Enable lighting
    glEnable(GL_LIGHT0);   // Enable light source 0

    // Define light properties
    GLfloat lightPosition[] = {0.0f, 0.0f, 0.0f, 1.0f}; // Position at (0, 0, 0)
    GLfloat lightAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};  // Ambient light
    GLfloat lightDiffuse[] = {1.0f, 1.0f, 0.8f, 1.0f};  // Diffuse light (soft yellow)
    GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Specular light (white)

    // Apply light properties
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}

void renderSolarSystem() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //updateCameraPosition(); 
    // Set up the camera
    gluLookAt(eyex, eyey, eyez,  // Camera position
              0.0, 0.0, 0.0,   // Look-at point
              0.0, 1.0, 0.0);  // Up direction

    // Draw the Sun
    glPushMatrix();
    glDisable(GL_LIGHTING); // Temporarily disable lighting for the Sun
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow Sun
    glutSolidSphere(2.0f, 50, 50); // Sun: radius 2
    glEnable(GL_LIGHTING); // Re-enable lighting
    glPopMatrix();

    //draw earth
    glPushMatrix();
    glRotatef(earthOrbitingAngle, 0.0f, 1.0f, 0.0f); // Earth's orbit around the Sun
    glTranslatef(10.0f, 0.0f, 0.0f); // Earth's orbit radius

    // Set Earth's material properties
    GLfloat earthAmbient[] = {0.0f, 0.0f, 0.3f, 1.0f};  // Ambient blue
    GLfloat earthDiffuse[] = {0.0f, 0.0f, 1.0f, 1.0f};  // Diffuse blue
    GLfloat earthSpecular[] = {0.2f, 0.2f, 0.2f, 1.0f}; // Specular highlights
    GLfloat earthShininess[] = {50.0f};                 // Shininess of the Earth
    glMaterialfv(GL_FRONT, GL_AMBIENT, earthAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, earthDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, earthSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, earthShininess);

    glutSolidSphere(1.0f, 50, 50); // Earth: radius 1

    //draw the moon
    glPushMatrix();
    glRotatef(moonOrbitAngle, 0.0f, 1.0f, 0.0f); // Moon's orbit around the Earth
    glTranslatef(3.0f, 0.0f, 0.0f); // Moon's orbit radius

    // Set Moon's material properties
    GLfloat moonAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};  // Ambient gray
    GLfloat moonDiffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};  // Diffuse gray
    GLfloat moonSpecular[] = {0.1f, 0.1f, 0.1f, 1.0f}; // Subtle specular highlights
    GLfloat moonShininess[] = {10.0f};                 // Shininess of the Moon
    glMaterialfv(GL_FRONT, GL_AMBIENT, moonAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, moonDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, moonSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, moonShininess);

    glutSolidSphere(0.3f, 50, 50); // Moon: radius 0.3
    glPopMatrix(); // End Moon transformation

    glutSwapBuffers(); // Swap buffers for double buffering

}

/*reshaping for the window*/
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

/*This updates based on the timescale*/
void update(int value) {
    earthOrbitingAngle += 0.5f; // Earth's orbit speed
    moonOrbitAngle += 2.0f;  // Moon's orbit speed
    if (earthOrbitingAngle >= 360.0f) earthOrbitingAngle -= 360.0f;
    if (moonOrbitAngle >= 360.0f) moonOrbitAngle -= 360.0f;

    glutPostRedisplay(); // Request display update
    glutTimerFunc(16, update, 0); // Call update every 16ms (~60 FPS)
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("N-Body Simulation");

    // Print OpenGL version
    const char* version = (const char*)glGetString(GL_VERSION);
    if (!version) {
        std::cerr << "Failed to retrieve OpenGL version." << std::endl;
        return -1;
    }
    std::cout << "OpenGL Version: " << version << std::endl;


    /*
        1.  Initialize
    
        2.  start the camera

        3.  set up lighting

        4.  display renderSolarSystem or the function that is requested

        5.  include reshape, keyboard, mousefunc, motionfunc, timerfunc and the main loop
    
    */
    
    Camera camera;
    camera.applyCameraView();
    /*sets up the lighting for the star*/
    setupLighting();                                                        // Set up the light source
    

    glutDisplayFunc(renderSolarSystem);                                     // Register the display callback

    /*changes the planets based off the screen size changing*/
    glutReshapeFunc(reshape);                                               // Register the reshape callback

    /*for the keyboard*/
    glutKeyboardFunc(keyboard);    

    /*recognizes mouse input*/
    glutMouseFunc(mouseButton);      // Register mouse button callback
    /*doing mouse functions based off the movement*/
    glutMotionFunc(mouseMotion);                                          
    
    glutTimerFunc(16, update, 0);                                           // Starts the timer function

    glutMainLoop();                                                         // Starts the main glu loop

    return 0;
}
