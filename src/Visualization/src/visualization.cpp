#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>       
#include <random>       //assigning colors




#include "body.h"
using namespace std;    //no more std::


/*
    What I need for my file
    I need to read in the input.txt file
*/
#include "FileReader.h"

int main() {
    try {
        FileReader reader("bodies.txt");
        auto bodies = reader.readBodies();

        for (const auto& body : bodies) {
            std::cout << "Body: Mass=" << body.getMass() 
                      << ", Position=" << body.getPosition() 
                      << ", Velocity=" << body.getVelocity() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}