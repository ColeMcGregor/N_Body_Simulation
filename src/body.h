#ifndef BODY_H
#define BODY_H

#include <cmath>
#include <iostream>
#include <string>
#include <vector>



struct Vector{
    double x,y,z;
    Vector(double x_ , double y_ , double z_);
    double magnitude()const;
    Vector operator+(const Vector& other)const;
    Vector operator-(const Vector& other)const;
    Vector operator*(double scalar)const;
    Vector operator/(double scalar)const;

};
class Body{
    public:
        Vector position; //where the body is
        Vector velocity; //how fast it is moving in a given direction
        Vector acceleration; //how fast it is accelerating in a given direction
        Vector angular_velocity; //how fast it is rotating
        double mass; //how much stuff it is made of
        double roll; //how much it is tilted around the x axis
        double pitch; //how much it is tilted around the y axis
        double yaw; //how much it is tilted around the z axis
        double density; //how much mass is in a given volume
        double radius; //how big it is from center to edge
        double oblateness; //how much it is squished from the poles to the equator
        string type; //what type of body it is(moon, planet, star, blackhole)
        Vector net_force; //the net force acting on the body

        //special variables
        double gravitationalMultiplier; //allows for different multiples of gravitational constants to see the effects of universal gravity scaling

        Body(Vector pos, Vector vel, Vector accel, Vector angularV, double mass, double roll, double pitch,double yaw, double density, double radius,
             double oblateness, string type, Vector net_force, double gravitationalMultiplier);
            
        Vector gravForce(const Body& p2) const;
        void applyForce(const Vector& force);
        void update(double timestep);
        Vector sumForces(const vector<Body>& bodies);


};
#endif