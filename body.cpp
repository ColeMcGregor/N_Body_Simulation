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
        
        double magnitude() const {
            return sqrt((x * x) + (y * y) + (z * z));
        }
};
/*
    Body class:
        Define a spheroid
            Vecored position
            vecored velocity
            vecored acceleration
            double mass
            double radius
            int oblateness (How squished or oblonged)
            (3 doubles Roll, pitch, yaw) represent Orientation
            Vectored angular velocity
            String type (Include moon, planet, star, blackhole)
            double density
        
*/
class Body{
    public:
        Vector position;
        Vector velocity;
        Vector acceleration;
        Vector angular_velocity;
        double mass;
        double roll;
        double pitch;
        double yaw;
        double density;
        double radius;
        int oblateness;
        string type;


        /*
            Body Constructor
            1) Give it everything
            2) Default constructors
                Dont have to feed it but it can generate itself
                Random mass, random positions

            every time we need:
                radius
                mass
                force

            we dont need every time:
                acceleration

            default:
                no feed
        */
        Body(Vector pos, Vector vel, Vector accel, Vector angularV, double mass, double roll, double pitch,double yaw, double density, double radius,
             int oblateness, string type){
            this->position = pos;
            this->velocity = vel;
            this->acceleration = accel;
            this->angular_velocity = angularV;
            this->mass = mass;
            this->roll = roll;
            this->pitch = pitch;
            this->yaw = yaw;
            this->density = density;
            this->radius = radius;
            this->oblateness = oblateness;
            this->type = type;

        }

        void setPos(Vector pos){
            this->position = pos;
        }
        void setVel(Vector vel){
            this->velocity = vel;
        }

        /*
            Calculate forces

            Gravitational force

            Param: Gravitational const, mass of B1, mass of B2, Distance between 2 bodys r, softening param e

            F = G(m1*m2/(pow(r,2)+ pow(e,2))

            Return : Vectored Force
        */
        

       /*
            Calculate vectored gravitational force

            Param: Position of Body 1, Position of Body 2

            Return : Vectored Magnitude
       */

      /*
            Sum the accumulated forces

            Params : 

                Vectered F = Sum of all Forces acting on body 1, between body 1 and all other bodys

            Return : Vectored Force
      */

     /*
            Apply vectored forces to acceleration

            Params : Vectored Force, Mass

            Vectored Acceleration  = Vector force / mass

            Return vectore acceleration
     */

    /*
            Apply acceleration to velocity and position using timestep
    */
};

int main() {

}