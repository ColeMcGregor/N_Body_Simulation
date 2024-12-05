#include <GL/glut.h>
#include <iostream>
#include <random>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
using namespace std;




/*
    This is my program that works in OpenGL, GLU, and creating a solar system representation for Parallel final project

        clang++ -std=c++11 -Iinclude -o  planet src/planet.cpp src/glad.c -lglfw -ldl -framework OpenGL -framework GLUT -framework GLUT -DGL_SILENCE_DEPRECATION;
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
    //Vector(int x_ = 0.0, int y_ = 0.0, int z_ = 0.0) : x(x_),y(y_),z(z_){}
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

/*struct for a star object*/
typedef struct star {
    Vector position;          // Position of the star
    float radius;             // Radius of the star
    float rotationalSpeed;    // Rotational speed
    float mass;               // Mass
    float luminosity;         // Luminosity
    Vector color;             // RGB color
    Vector velocity;          // Velocity vector
    Vector netForce;          // Net force acting on the star
    Vector acceleration;      // Acceleration
    std::string name;         // Name of the star
    int star_id;

    star(){}
    star(Vector position, float radius, float rotationalSpeed, float mass,
         float luminosity, Vector color, Vector velocity, Vector netForce,
         Vector acceleration, std::string name, int star_id)
        : position(position), radius(radius), rotationalSpeed(rotationalSpeed),
          mass(mass), luminosity(luminosity), color(color), velocity(velocity),
          netForce(netForce), acceleration(acceleration), name(name),star_id(star_id) {}

    // Getters
    Vector getPosition() const { return position; }
    float getRadius() const { return radius; }
    float getRotationalSpeed() const { return rotationalSpeed; }
    float getMass() const { return mass; }
    float getLuminosity() const { return luminosity; }
    Vector getColor() const { return color; }
    Vector getVelocity() const { return velocity; }
    Vector getNetForce() const { return netForce; }
    Vector getAcceleration() const { return acceleration; }
    std::string getName() const { return name; }
    int getStar_id() const{return star_id;}

    // Setters
    void setPosition(const Vector& pos) { position = pos; }
    void setRadius(float rad) { radius = rad; }
    void setRotationalSpeed(float speed) { rotationalSpeed = speed; }
    void setMass(float m) { mass = m; }
    void setLuminosity(float lum) { luminosity = lum; }
    void setColor(const Vector& col) { color = col; }
    void setVelocity(const Vector& vel) { velocity = vel; }
    void setNetForce(const Vector& force) { netForce = force; }
    void setAcceleration(const Vector& accel) { acceleration = accel; }
    void setName(const std::string& nm) { name = nm; }
    void setStar_id(const int sid){star_id = sid;};

    // Display star properties
    void display() const {
        cout << "Star Name: " << name << std::endl;
        cout << "Position: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
        std::cout << "Radius: " << radius << std::endl;
        std::cout << "Rotational Speed: " << rotationalSpeed << std::endl;
        std::cout << "Mass: " << mass << std::endl;
        std::cout << "Luminosity: " << luminosity << std::endl;
        std::cout << "Color: (" << color.x << ", " << color.y << ", " << color.z << ")" << std::endl;
        std::cout << "Velocity: (" << velocity.x << ", " << velocity.y << ", " << velocity.z << ")" << std::endl;
        std::cout << "Net Force: (" << netForce.x << ", " << netForce.y << ", " << netForce.z << ")" << std::endl;
        std::cout << "Acceleration: (" << acceleration.x << ", " << acceleration.y << ", " << acceleration.z << ")" << std::endl;
    }
}star;

/*struct for a planet object*/
struct planet {
    planet() = default;
    planet(Vector position, Vector netForce, Vector velocity, Vector color,
           Vector acceleration,
           float radius, float rotationalSpeed, float mass, float rotationalPeriod,
           float orbitalPeriod, int planet_id, std::string name,int starID)
        : position(position), netForce(netForce), velocity(velocity), color(color),
          radius(radius), rotationalSpeed(rotationalSpeed), mass(mass),
          rotationalPeriod(rotationalPeriod), orbitalPeriod(orbitalPeriod),
          planet_id(planet_id), name(name),starID(starID) {}

    Vector position;            // Position of the planet
    Vector netForce;            // Net force currently acting on the planet (used to calculate acceleration)
    Vector velocity;            // Velocity of the planet
    Vector acceleration;        //acceleration
    Vector color;               // RGB color of the planet for visualization
    float radius;               // Radius of the planet
    float rotationalSpeed;      // Rotational speed of the planet
    float mass;                 // Mass of the planet
    float rotationalPeriod;     // Time taken to rotate 360 degrees (in some time unit)
    float orbitalPeriod;        // Time taken to complete one orbit around the star
    int planet_id;              // Unique ID for the planet (can be used for moons or identification)
    std::string name;           // Name of the planet
    int starID;

    // Display star properties
    void display() const {
        cout << "Planet Name: " << name << std::endl;
        cout << "Position: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
        std::cout << "Radius: " << radius << std::endl;
        std::cout << "Rotational Speed: " << rotationalSpeed << std::endl;
        std::cout << "Mass: " << mass << std::endl;
        std::cout << "Color: (" << color.x << ", " << color.y << ", " << color.z << ")" << std::endl;
        std::cout << "Velocity: (" << velocity.x << ", " << velocity.y << ", " << velocity.z << ")" << std::endl;
        std::cout << "Net Force: (" << netForce.x << ", " << netForce.y << ", " << netForce.z << ")" << std::endl;
        std::cout << "Acceleration: (" << acceleration.x << ", " << acceleration.y << ", " << acceleration.z << ")" << std::endl;
    }
};

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
star *stars = nullptr;
planet *planets = nullptr;
int numStars = 0;
int numPlanets = 0;
int numMoons = 0;
int numBodies = 0;
int bodies_left = 0;

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

default_random_engine engine;
random_device rd;


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

void setMaterialProperties(const planet& planet) {
    GLfloat setAmbient[] = {planet.color.x * 0.1f, planet.color.y * 0.1f, planet.color.z * 0.1f, 1.0f};  // Less intense ambient
    GLfloat setDiffuse[] = {planet.color.x, planet.color.y, planet.color.z, 1.0f};  // Diffuse color
    GLfloat setSpecular[] = {planet.color.x * 0.3f, planet.color.y * 0.3f, planet.color.z * 0.3f, 1.0f}; // Specular with lower intensity
    GLfloat setShininess[] = {30.0f};                 // Shininess of the planet
    glMaterialfv(GL_FRONT, GL_AMBIENT, setAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, setDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, setSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, setShininess);
}

void renderSolarSystem() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //updateCameraPosition(); 
    // Set up the camera
    gluLookAt(eyex, eyey, eyez,  // Camera position
              0.0, 0.0, 0.0,   // Look-at point
              0.0, 1.0, 0.0);  // Up direction

    glPushMatrix();
    glDisable(GL_LIGHTING); // Temporarily disable lighting for the Sun

    for (int i = 0; i < numStars; i++) {
        glColor3f(stars[i].color.x, stars[i].color.y, stars[i].color.z);
        glPushMatrix();
        glTranslatef(stars[i].position.x, stars[i].position.y, stars[i].position.z);
        glutSolidSphere(stars[i].radius, 100, 100);
        glPopMatrix();
    }

    glEnable(GL_LIGHTING);
    
    for(int i = 0 ; i < numPlanets; i++){
        glColor3f(planets[i].color.x,planets[i].color.y,planets[i].color.z);
        glPushMatrix();
        //glRotatef(orbitingAngle  ,0.0f,1.0f,0.0f);  orbit around the star
        glTranslatef(planets[i].position.x, planets[i].position.y,planets[i].position.z);//orbit radius

        setMaterialProperties(planets[i]);

        glutSolidSphere(planets[i].radius,100,100);

    }
    glPopMatrix();


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

Vector starColors[7]{
    Vector(155.0/255.0f, 176.0/255.0f, 255.0/255.0f),      //O color star
    Vector(170.0/255.0f, 191.0/255.0f, 255.0/255.0f),      //B color star
    Vector(202.0/255.0f, 215.0/255.0f, 255.0/255.0f),      //A color star
    Vector(248.0/255.0f, 247.0/255.0f, 255.0/255.0f),      //F color star
    Vector(255.0/255.0f, 244.0/255.0f, 234.0/255.0f),      //G color star
    Vector(255.0/255.0f, 210.0/255.0f, 161.0/255.0f),      //K color star
    Vector(255.0/255.0f, 204.0/255.0f, 111.0/255.0f)       //M color star
    };
Vector planetColors[8]{
    Vector{191.0f / 255.0f, 121.0f / 255.0f, 42.0f / 255.0f},   // Mercury - Grayish brown
    Vector{241.0f / 255.0f, 163.0f / 255.0f, 79.0f / 255.0f},    // Venus - Pale yellow-orange
    Vector{70.0f / 255.0f, 130.0f / 255.0f, 180.0f / 255.0f},    // Earth - Blue
    Vector{188.0f / 255.0f, 39.0f / 255.0f, 50.0f / 255.0f},     // Mars - Reddish
    Vector{255.0f / 255.0f, 223.0f / 255.0f, 150.0f / 255.0f},   // Jupiter - Orange and white bands
    Vector{219.0f / 255.0f, 207.0f / 255.0f, 137.0f / 255.0f},   // Saturn - Pale yellow
    Vector{175.0f / 255.0f, 238.0f / 255.0f, 238.0f / 255.0f},   // Uranus - Light cyan
    Vector{25.0f / 255.0f, 25.0f / 255.0f, 112.0f / 255.0f}      // Neptune - Dark blue
    };

star createStar(){
    engine.seed(rd());
    uniform_int_distribution<int> intDist(0, 6);
    uniform_real_distribution<float> position(-20,20);
    uniform_real_distribution<float> radius(0,1);
    star curr(
        //Vector(0.0,0.0,0.0),
        Vector(position(engine),position(engine),position(engine)),/*random 3 ints for position, lets start at 0*/
        radius(engine),
        1.0,
        2.0,
        2.0,
        starColors[intDist(engine)],
        Vector(0.0,0.0,0.0),
        Vector(0.0,0.0,0.0),
        Vector(0.0,0.0,0.0),
        "sun",
        1
    );
    return curr;
}

planet createPlanet(star star) {
    static std::random_device rd;
    static std::mt19937 engine(rd());
    std::uniform_int_distribution<int> intDist(0, 7);
    std::uniform_real_distribution<float> radius (0,5);

    planet curr(
        //position needs to be its star position plus their position
        Vector(10.0, 0.0, 0.0), // Position: puts the planet 10 units on the x-axis away from the sun
        Vector(0.0, 0.0, 0.0),  // Net force: initially zero
        Vector(0.0, 0.0, 0.0),  // Velocity: initially zero
        Vector(0.0,0.0,0.0),
        planetColors[intDist(engine)], // Random color from planetColors array
        2.0,                    // Radius of the planet
        1.0,                    // Rotational speed of the planet
        1.0,                    // Mass of the planet
        1.0,                    // Rotational period
        1.0,                    // Orbital period
        1,                      // Unique planet ID
        "planet",               // Name of the planet
        star.star_id
    );
    return curr;
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

    

    
    cout <<"How many bodies do you want?"<<endl;
    cin >> numBodies;
    bodies_left = numBodies;

    cout<<"How many stars out of " << bodies_left << endl;
    cin>> numStars;
    bodies_left -= numStars;

    cout<<"How many planets out of " << bodies_left <<endl;
    cin >>numPlanets;
    bodies_left -= numPlanets;

    cout<<"We will have " << numStars << " stars and "<<numPlanets<<" planets with the rest being moons"<<endl;

    stars = new star[numStars];
    planets = new planet[numPlanets];

    // Draw the Sun
    if(numStars != 0){
        for(int i = 0 ; i < numStars ; i++){
            stars[i] = createStar();
            cout<<"Creating star " << i << endl;
        }
    }
    
    if(numPlanets != 0){
        for(int i = 0 ; i < numPlanets ; i++){
            planets[i] = createPlanet(stars[0]);
            cout<<"Printing out planet "<< i <<endl;
        }
    }
    cout<<"Starting Lighting"<<endl;
    setupLighting();
    cout<<"Starting renderSolarSystem"<<endl;
    glutDisplayFunc(renderSolarSystem);                                     // Register the display callback

    /*changes the planets based off the screen size changing*/
    glutReshapeFunc(reshape);                                               // Register the reshape callback
    glutKeyboardFunc(keyboard);   

    glutMainLoop();                                                         // Starts the main glu loop
    return 0;
}
