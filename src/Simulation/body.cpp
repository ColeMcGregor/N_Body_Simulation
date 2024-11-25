#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include "body.h"
using namespace std;

/**
 *  A VECTOR IS NOT THE SAME AS THE CPP VECTOR CLASS
 * IT HOLDS X Y Z COORDINATES AND HAS METHODS TO MANIPULATE THEM
 * A CPP vector is a dynamic array of objects
 * PLEASE DO NOT CONFUSE THE TWO
 * 
 * 
 */

//constructor for the Vector struct, defaults to 0 (0,0,0) the origin
    Vector::Vector(double x_, double y_, double z_)
    : x(x_), y(y_), z(z_) {}
    /*
        Calculate the magnitude of the vector(length of space between the origin and the point, ignoring direction)
    */
    double Vector::magnitude() const {
        return sqrt((x * x) + (y * y) + (z * z));
    }
    //perform vector addition, returns a new vector
    Vector Vector::operator+(const Vector& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }
    //perform vector subtraction, returns a new vector
    Vector Vector::operator-(const Vector& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }
    //perform scalar multiplication, returns a new vector, a scalar is a single number(not a vector) used to "scale" the vector
    Vector Vector::operator*(double scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }
    //perform scalar division, returns a new vector, a scalar is a single number(not a vector) used to "scale" the vector
    Vector Vector::operator/(double scalar) const {
        return {x / scalar, y / scalar, z / scalar};
    }
    //print the vector with commas between the values
    void Vector::print() const {
        cout << x << "," << y << "," << z << endl;
    }

/*
    Body class:
        Define a spheroid
            Vectored position
            Vectored velocity
            Vectored acceleration
            Vectored angular velocity
            Vectored net force
            double mass          
            double density
            double radius
            double oblateness (How squished or oblonged)
            (3 doubles Roll, pitch, yaw) represent Orientation
            String type (Include moon, planet, star, blackhole)

*/
/*
    Constructor for the Body class
*/
Body::Body( Vector pos, 
            Vector vel,
            Vector accel, 
            Vector angularV, 
            Vector net_force,
            double mass,  
            double density,
            double radius, 
            double oblateness, 
            double gravitationalMultiplier,
            double roll, 
            double pitch, 
            double yaw, 
            string type)
            :   
            position(pos), 
            velocity(vel), 
            acceleration(accel), 
            angular_velocity(angularV),
            net_force(net_force), 
            mass(mass), 
            density(density), 
            radius(radius),
            oblateness(oblateness), 
            gravitationalMultiplier(gravitationalMultiplier),
            roll(roll), 
            pitch(pitch), 
            yaw(yaw), 
            type(type) {}


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
    const double epsilon = 1e-5;  // Softening parameter to limit the force at very close distances

    // Compute the distance vector
    Vector r(p2.position.x - position.x, p2.position.y - position.y, p2.position.z - position.z); //the vectored distance between the two bodies
    double dist = r.magnitude(); //the magnitude of the distance between the two bodies

    // Compute gravitational force magnitude
    double forceMag = (G * gravitationalMultiplier) * (mass * p2.mass) / ((dist * dist) + (epsilon * epsilon));

    // Normalize r and scale by force magnitude
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
    net_force = Vector(0, 0, 0);
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

int main()
{
    // Test Case 1: Earth and Moon
    Body earth(Vector(), Vector(), Vector(), Vector(), 5.972e24, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, "earth");
    Body moon(Vector(3.84e8, 0, 0), Vector(), Vector(), Vector(), 7.35e22, 0, 0, 0, 3344, 1.0, 1737e3, 0.0, 0.0, "moon");

    vector<Body> bodies = {earth, moon};

    for (Body &body : bodies)
    {
        body.sumForces(bodies);
    }

    for (const Body &body : bodies)
    {
        body.printState();
    }

    return 0;
}