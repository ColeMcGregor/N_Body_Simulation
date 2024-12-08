#ifndef BODY_H
#define BODY_H


#include "vector.h"
#include <string>

using namespace std;    //no more std::

class Body{
    protected:
        double radius;      //every body will have a radius
        double mass;        //mass of the planets
        vector<Vector>  position;    //every body will have a position
        string type;        //type of body
    public:
        /*spot for constructor*/
    Body(double radius,double mass, const Vector& position, const string& type);

    // const Vector& getPosition() const{
    //     return position;
    // }
    const double getRadius() const{
        return radius;
    }
};

#endif