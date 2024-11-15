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

        /*
            Setter functions
        */
        void setPos(Vector pos){
            this->position = pos;
        }
        void setVel(Vector vel){
            this->velocity = vel;
        }
        void setAcceleration(Vector accl){
            this->acceleration = accl;
        }
        void setAngularV(Vector angV){
            this->angular_velocity = angV;
        }
        void setMass(double m){
            this->mass = m;
        }
        void setRoll(double r){
            this->roll = r;
        }
        void setPitch(double p){
            this->pitch = p;
        }
        void setYaw(double y){
            this->yaw = y;
        }
        void setDensity(double d){
            this->density = d;
        }
        void setRadius(double r){
            this->radius = r;
        }
        void setOblateness(int o){
            this->oblateness = o;
        }
        void setType(string t){
            this->type = t;
        }

        /*
            Getter Functions
        */
        Vector getPos(){return this->position;}
        Vector getVel(){return this->velocity;}
        Vector getAccel(){return this->acceleration;}
        Vector getAngular(){return this->angular_velocity;}
        double getMass(){return this->mass;}
        double getRoll(){return this->roll;}
        double getPitch(){return this->pitch;}
        double getYaw(){return this->yaw;}
        double getDensity(){return this->density;}
        double getRadius(){return this->radius;}
        int getOblateness(){return this->oblateness;}
        string getType(){return this->type;}
        /*
            Calculate forces

            Gravitational force

            Param: Gravitational const, mass of B1, mass of B2, Distance between 2 bodys r, softening param e

            F = G(m1*m2/(pow(r,2)+ pow(e,2))

            Return : Vectored Force
        */
       Vector gravForce( Body p2){
       const double GRAVITATIONAL_CONSTANT = 6.67430e-11; // m^3 kg^-1 s^-2

        //step one is find the dist
        /*
            Find the dist
            use p1, x1,y1,z1 and p2, x2,y2,z2 to get the distance
        */
        double dist = sqrt(pow(p2.position.x - this->position.x,2) + pow(p2.position.y - this->position.y,2) + pow(p2.position.z - this->position.z,2));
        vector temp = GRAVITATIONAL_CONSTANT((this->mass * p2.mass)/pow(dist,2));
        return temp;
       }
        

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
    // Test Case 1: Earth and Moon
    Body earth(Vector(0, 0, 0), Vector(), Vector(), Vector(), 5.97e24, 0, 0, 0, 5514, 6371e3, 0, "planet");
    Body moon(Vector(3.84e8, 0, 0), Vector(), Vector(), Vector(), 7.35e22, 0, 0, 0, 3344, 1737e3, 0, "moon");

    Vector forceEarthOnMoon = earth.gravForce(moon, earth);
    cout << "Gravitational Force on Moon by Earth: "
         << "Fx: " << forceEarthOnMoon.x << " N, "
         << "Fy: " << forceEarthOnMoon.y << " N, "
         << "Fz: " << forceEarthOnMoon.z << " N" << endl;

    // Test Case 2: Sun and Earth
    Body sun(Vector(0, 0, 0), Vector(), Vector(), Vector(), 1.989e30, 0, 0, 0, 1408, 695700e3, 0, "star");
    Vector forceSunOnEarth = sun.gravForce(earth, moon);
    cout << "Gravitational Force on Earth by Sun: "
         << "Fx: " << forceSunOnEarth.x << " N, "
         << "Fy: " << forceSunOnEarth.y << " N, "
         << "Fz: " << forceSunOnEarth.z << " N" << endl;

    // Test Case 3: Identical position (should return error)
    Body samePosition(Vector(0, 0, 0), Vector(), Vector(), Vector(), 1e5, 0, 0, 0, 1000, 1e3, 0, "test");


    return 0;
}