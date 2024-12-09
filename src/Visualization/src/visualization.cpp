#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <random>       // Assigning colors

// #include "vector.h"
// //#include "Body.h"
// #include "Star.h"
// #include "Planet.h"
// #include "Moon.h"
// #include "BlackHole.h"
// #include "FileReader.h"

using namespace std;

/*
    g++ -std=c++11 -o vis visualization.cpp FileReader.cpp Body.cpp Star.cpp Planet.cpp Moon.cpp BlackHole.cpp vector.cpp -framework OpenGL -framework GLUT -I/usr/local/include
*/


//graphics again dont worry
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
} 


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
struct Body {
    int id;
    std::string type;
    double radius;
    std::vector<std::vector<double>> positions;
};


vector<Body>bodies;
int timestep = 0;

// Initialize OpenGL
void initOpenGL() {
    glEnable(GL_DEPTH_TEST);         // Enable depth testing
    glDepthFunc(GL_LEQUAL);          // Less-than-or-equal depth test
    glClearColor(0.0, 0.0, 0.0, 1.0); // Black background

}



// void drawBody(const Body& body, const std::vector<double>& position) {
//     glPushMatrix();
//     glTranslatef(position[0] * 1e6, position[1] * 1e6, position[2] * 1e6); // Scale positions for visibility

//     if (body.type == "star")
//         glColor3f(1.0, 1.0, 0.0); // Yellow for stars
//     else if (body.type == "planet")
//         glColor3f(0.0, 0.0, 1.0); // Blue for planets
//     else if (body.type == "moon")
//         glColor3f(0.5, 0.5, 0.5); // Gray for moons
//     else
//         glColor3f(1.0, 0.0, 0.0); // Red for other types

//     glutSolidSphere(body.radius / 1e9, 20, 20); // Scale radius for visualization
//     glPopMatrix();
// }


double zpos = 50;
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': zpos -= 50.0f; //need to be fixed
            if(zpos < 1.0){
                zpos = 1.0;
            } break; // Move closer
        case 's': zpos += 50.f; 
              break; // Move farther  //needs to be fixed
        case  27: exit(0); break;   //esc key           //works
        // case '+': zoomFactor += 0.1f;                   //needs to be fixed
        //     std::cout <<"Zooming in" << ::std::endl; break;
        // case '-': zoomFactor -= 0.1f;                   //needs to be fixed
        //     std::cout <<"Zooming out" << ::std::endl; break;
        
    }
    glutPostRedisplay(); // Redraw the scene
}
// void renderScene() {
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffers
//     glLoadIdentity();

//     // Set camera position
//     gluLookAt(0.0, 0.0, 10.0,   // Camera position (z = 10)
//               0.0, 0.0, 0.0,    // Look-at point
//               0.0, 1.0, 0.0);   // Up vector

//     // Render each body
//     for (const auto& body : bodies) {
//         if (body.positions.empty()) {
//             // Skip rendering if there are no positions
//             std::cerr << "Warning: Body " << body.id << " has no positions." << std::endl;
//             continue;
//         }

//         // Ensure timestep is valid for the current body
//         size_t currentIndex = timestep % (body.positions.size() - 1);
//         size_t nextIndex = currentIndex + 1;

//         // Interpolation factor (0.0 to 1.0)
//         float alpha = static_cast<float>(timestep % 100) / 100.0f;

//         // Interpolate position: (1 - alpha) * current + alpha * next
//         std::vector<double> interpolatedPosition(3);
//         for (size_t i = 0; i < 3; ++i) {
//             interpolatedPosition[i] =
//                 (1.0 - alpha) * body.positions[currentIndex][i] +
//                 alpha * body.positions[nextIndex][i];
//         }

//         // Draw the body at the interpolated position
//         drawBody(body, interpolatedPosition);
//     }

//     glutSwapBuffers(); // Swap buffers to display the rendered frame
// }


#include <algorithm>

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
}

void update(int value) {
    timestep++;

    // Ensure timestep doesn't exceed the largest `positions.size()`
    size_t maxTimestep = 0;
    for (const auto& body : bodies) {
        maxTimestep = std::max(maxTimestep, body.positions.size());
    }
    if (timestep >= maxTimestep) {
        timestep = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // Call again in 16ms
}


#include "FileReader.h"

int main(int argc, char** argv) {


    FileReader reader("output.txt");
    int timestep = reader.readTimeStep();
    auto[localBodies,localStars,localPlanets,localMoons,localBH] = reader.readBodies();

    // Debugging Output
    std::cout << "Timestep: " << timestep << std::endl;
    //std::cout << "Number of Bodies: " << numBodies << std::endl;


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Celestial Bodies Visualization");

    initOpenGL();

    //glutDisplayFunc(renderScene);
    glutTimerFunc(16, update, 0); // Start animation loop

    glutKeyboardFunc(keyboard);
    glutMainLoop();
}










    // for (const auto& group : hierarchy) {
    //     std::cout << "Hierarchy: ";
    //     for (int id : group) std::cout << id << " ";
    //     std::cout << std::endl;
    // }

    // for (const auto& body : bodies) {
    //     std::cout << "Body ID: " << body.id << ", Type: " << body.type << ", Radius: " << body.radius << std::endl;
    //     for (const auto& position : body.positions) {
    //         std::cout << "Position: (" << position[0] << ", " << position[1] << ", " << position[2] << ")" << std::endl;
    //     }
    // }


//     //graphics from here down
//         glutInit(&argc,argv);
//         glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//         glutInitWindowSize(800, 600);
//         glutCreateWindow("N-Body Simulation");
//         // glutReshapeFunc(reshape); 



//         // glMatrixMode(GL_PROJECTION);
//         // glLoadIdentity();
//         // gluPerspective(45.0,800.0/600.0, 1.0,200.0);

//         // glMatrixMode(GL_MODELVIEW);
//         // glLoadIdentity();

//         // calculateScale();

//         glutReshapeFunc(reshape);
//         glutDisplayFunc(display); // Register display callback 
//         glutMainLoop();
        
//     } catch (const std::exception& e) {
//         cerr << "Error: " << e.what() << endl;
//     }

//     return 0;
// }
