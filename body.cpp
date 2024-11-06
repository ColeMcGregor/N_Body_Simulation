#include <cmath>
#include <iostream>


/*
    This will have all the properties of a planet body
*/
class Vector{
    public:
        double x,y,z; // these will be where the planets are moving
        Vector(double x_ = 0 ,double y_ = 0,double z_ = 0): x(x_),y(y_),z(z_){} //if no params are given, default is zero
};
class Body{
    public:
        double mass;
        Vector position;
        Vector velocity;
        Vector force;

        //going to create a body now, sets mass, position, velocity, and force
        Body(double m, Vector p, Vector v): mass(m), position(p),velocity(v),force(0,0,0) {};

        //body functions that can be used
        void resetForce(){
            force = Vector(0,0,0);
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
        void updatePosition(double mass,Body position,Body velocity, Body force){
            //update position using velocity and force
        }
        

};