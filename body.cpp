#include <cmath>
#include <iostream>
using namespace std;


/*
    Vector struct for holding x,y,z coordinates, 
    has a magnitude function, and a constructor that defaults to 0
    has operator overloading for +, -, *, and / to allow for vector math

    Vector is used to represent position, velocity, acceleration, and force
    as they are all vectored quantities


*/
struct Vector {
    double x, y, z;

    Vector(double x_ = 0.0, double y_ = 0.0, double z_ = 0.0)
        : x(x_), y(y_), z(z_) {}

    /*
        Calculate the magnitude of the vector(length of space between the origin and the point, ignoring direction)
    */
    double magnitude() const {
        return sqrt((x * x) + (y * y) + (z * z));
    }
    //perform vector addition, returns a new vector
    Vector operator+(const Vector& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }
    //perform vector subtraction, returns a new vector
    Vector operator-(const Vector& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }
    //perform scalar multiplication, returns a new vector, a scalar is a single number(not a vector) used to "scale" the vector
    Vector operator*(double scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }
    //perform scalar division, returns a new vector, a scalar is a single number(not a vector) used to "scale" the vector
    Vector operator/(double scalar) const {
        return {x / scalar, y / scalar, z / scalar};
    }
};

/*
    Body class:
        Define a spheroid
            Vectored position
            Vectored velocity
            Vectored acceleration
            double mass
            double radius
            double oblateness (How squished or oblonged)
            (3 doubles Roll, pitch, yaw) represent Orientation
            Vectored angular velocity
            String type (Include moon, planet, star, blackhole)
            double density
        
*/
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

        //special variables
        double gravitationalMultiplier; //allows for different multiples of gravitational constants to see the effects of universal gravity scaling


        /*
            Constructor for the Body class
        */
        Body(Vector pos, Vector vel, Vector accel, Vector angularV, double mass, double roll, double pitch,double yaw, double density, double radius,
             double oblateness, string type, double gravitationalMultiplier){
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
            this->gravitationalMultiplier = gravitationalMultiplier;
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
        void setOblateness(double o){
            this->oblateness = o;
        }
        void setType(string t){
            this->type = t;
        }
        void setGravitationalMultiplier(double gm){
            this->gravitationalMultiplier = gm;
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
        double getOblateness(){return this->oblateness;}
        string getType(){return this->type;}
        double getGravitationalMultiplier(){return this->gravitationalMultiplier;}
        /*
            Calculate forces

            Gravitational force

            Param: Gravitational const, mass of B1, mass of B2, Distance between 2 bodys r, softening param e

            F = G(m1*m2/(pow(r,2)+ pow(e,2))

            Return : Vectored Force
        */
        Vector gravForce(const Body& p2) const {
            const double G = 6.67430e-11; // Gravitational constant
            const double epsilon = 1e-5;  // Softening parameter

            // Compute the distance vector
            Vector r = {p2.position.x - position.x, p2.position.y - position.y, p2.position.z - position.z};
            double dist = r.magnitude();

            // Compute gravitational force magnitude
            double forceMag = (G* gravitationalMultiplier) * (mass * p2.mass) / ((dist * dist) + (epsilon * epsilon));

            // Normalize r and scale by force magnitude
            return {r.x / dist * forceMag, r.y / dist * forceMag, r.z / dist * forceMag};
        }

    
        /**
         * Apply a force to the body, used to calculate acceleration
         * 
         * 
         * 
         * @param force the force to apply
         * @return void
         */
        void applyForce(const Vector& force) {
            acceleration = force / mass;
    }

        void update(double timestep) {
            velocity = velocity + acceleration * timestep;
            position = position + velocity * timestep;
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

    double forceEarthOnMoon = earth.gravForce(moon);
    cout << "Gravitational Force on Moon by Earth: " << forceEarthOnMoon << endl;


    return 0;
}