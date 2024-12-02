#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include "body.h"
#include "vector.h"
using namespace std;


/*
    Body class:
        Define a spheroid
            Vectored position
            Vectored velocity
            Vectored acceleration
            Vectored net force
            double mass          
            double density
            double radius
            String type (Include moon, planet, star, blackhole maybe)
            int[] childrenIndices
            std::vector<Vector> trajectory

*/
/*
    Constructor for the Body class
*/
Body::Body(
    const Vector& pos,                                                              // Position
    const Vector& vel,                                                              // Velocity
    const Vector& accel = Vector(),                                                 // Acceleration defaults to (0, 0, 0)
    const Vector& net_force = Vector(),                                             // Net force defaults to (0, 0, 0)
    double mass,                                                                    // Mass
    double density,                                                                 // Density
    double radius,                                                                  // Radius
    double gravitationalMultiplier,                                                 // Gravitational multiplier
    const std::string& type,                                                        // Type of body
    const std::vector<int>& childrenIndices = {},                                   // Default empty vector for children indices
    const std::vector<Vector>& trajectory = {}                                      // Default empty trajectory
)
    : position(pos),
      velocity(vel),
      acceleration(accel),
      net_force(net_force),
      mass(mass),
      density(density),
      radius(radius),
      gravitationalMultiplier(gravitationalMultiplier),
      type(type),
      childrenIndices(childrenIndices),
      trajectory(trajectory) {}


/*
    Calculate forces
    Gravitational force between two bodies
    Param: Gravitational const, mass of B1, mass of B2, Distance between 2 bodys r, softening param e
    F = G( (m1*m2) / ((r*r) + (e*e)))
    Return : Vectored Force
*/
Vector Body::gravForce(const Body &p2) const
{
    const double G = 6.67430e-11; // Predefined and recognized Gravitational constant
    const double epsilon = 1e-5;  // Softening parameter to limit the force at very close distances (0.00001)

    // Compute the distance vector
    Vector r(p2.position.x - position.x, p2.position.y - position.y, p2.position.z - position.z); //the vectored distance between the two bodies
    double dist = r.magnitude(); //the magnitude of the distance between the two bodies
    if (dist < epsilon) {
        dist = epsilon; // Prevent divide-by-zero or extremely large forces
    }
    // Compute gravitational force magnitude
    double forceMag = (G * gravitationalMultiplier) * (mass * p2.mass) / ((dist * dist) + (epsilon * epsilon));

    // Normalize r(distance from one body to the other, ignoring dimensions) and scale by force magnitude
    return Vector(r.x / dist * forceMag, r.y / dist * forceMag, r.z / dist * forceMag); //the vectored force between the two bodies
}

/**
 * Apply a force to the body, used to calculate acceleration
 * will use vector math to apply the force
 * acceleration(vectored) = force(vectored) / mass(Scalar)
 *
 *
 * @param force the force to apply
 * @return void
 */
void Body::applyForce(const Vector &force)
{
    // calculate acceleration change
    Vector accelChange = force / this->mass;
    // update acceleration
    this->acceleration = this->acceleration + accelChange;
}
/**update the velocity and position of the body using a timestep(we define the time step to accelerate the simulation)
 * This will be used to update the position of the body over time
 * THIS IS A VERY IMPORTANT FUNCTION
 *
 *
 * @param timestep the amount of time to update the body over
 * @return void
 */

void Body::update(double timestep)
{
    // update velocity and position
    this->velocity = this->velocity + (this->acceleration * timestep);
    this->position = this->position + this->velocity * timestep;

    // reset acceleration for next timestep
    this->acceleration = Vector(0, 0, 0);
}

/*
      Sum the accumulated forces

      Params : Vector of all bodys

      Return : Vectored Sum of all Forces acting on body n, between body n and all other bodys
*/
Vector Body::sumForces(const vector<Body> &bodies)
{
    // always reset the net force before each calculation
    net_force = Vector(0, 0, 0);
    // loop through all bodies and calculate the force between this body and the other bodies
    for (const Body &body : bodies)
    {
        // avoid calculating the force between itself
        if (this != &body)
        {
            // accumulate the force to the net force
            net_force = net_force + gravForce(body);
        }
    }
    return net_force;
}
void Body::resetForce() {
    net_force.reset(); //reset the net force to (0, 0, 0) using the reset method in vector.h
}


/*
        Apply acceleration to velocity and position using timestep
*/

// debug method for testing
void Body::printState() const
{
    //will print the state of the body to the console using the vector print method
    cout << "Type: " << type << ", Position: ";
    position.print();
    cout << "Velocity: ";
    velocity.print();
    cout << "Acceleration: ";
    acceleration.print();
}


