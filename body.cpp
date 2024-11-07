#include <cmath>
#include <iostream>
using namespace std;


/*
    This will have all the properties of a planet body
*/
class Vector{
    public:
        double x,y,z; // these will be where the planets are moving
        Vector(double x_ = 0.0 ,double y_ = 0.0,double z_ = 0.0): x(x_),y(y_),z(z_){} //if no params are given, default is zero
};
class Body{
    public:
        double mass;
        Vector position;
        Vector velocity;
        Vector force;
        static double timeStep = 0.01;

        //going to create a body now, sets mass, position, velocity, and force
        Body(double m, Vector p, Vector v): mass(m), position(p), velocity(v), force(0.0, 0.0, 0.0) {
            if (m <= 0) {
                throw invalid_argument("Mass must be greater than 0");
            }
        }

        //body functions that can be used
        void resetForce(){
            force = Vector(0.0, 0.0, 0.0);
        }
        /*
            Calculating the distance vector
            vector r = vector r_other - vector r_self
            or 
            vector r = (x2-x1,y2-y1,z2-z1)

            where r_other is the vector position of the other body
            where r_self is the position of the current body



            Newton's Law of Universal Graitation
            F=G * (M1M2/r^2)

            Magnitude of the Distance
            double r = SQRT((x2 - x1)^2 + (y2-y1)^2+(z2-z1)^2)

            Unit direction Vector
            vector r = (x2-x1/r,y2-y1/r,z2-z1/r)
            We will divide each component of the vector by the total distance to get the direction of force
        */
        void applyForce(Body p2){
            //step one is to compute the distance between 2 bodys
            Vector dist = findDist( *this, p2);

            Vector mag = sqrt(pow(x2 - x1,2)+ pow(y2-y1,2)+ pow(z2-z1,2));
        }

        Vector findDist(Body p1, Body p2){
            Vector dist = Vector((p2.position.x - p1.position.x),
                                 (p2.position.y - p1.position.y),
                                 (p2.position.z - p1.position.z));
            return dist;
        }

        void updatePosition(){
            // calculate acceleration (a = F/m)
            Vector accel = Vector(
                force.x / mass,
                force.y / mass,
                force.z / mass
            );

            /* update position
                position formula:
                let s = position,
                    s = s0 + v0*t + (0.5*a*t^2)
            */
            position.x += velocity.x * timeStep + (0.5 * accel.x * timeStep * timeStep);
            position.y += velocity.y * timeStep + (0.5 * accel.x * timeStep * timeStep);
            position.z += velocity.z * timeStep + (0.5 * accel.x * timeStep * timeStep);

            /* update velocity
                velocity formula:
                    v = v0 + a*t
            */
           velocity.x += accel.x * timeStep;
           velocity.y += accel.y * timeStep;
           velocity.z += accel.z * timeStep;
        }
        

};