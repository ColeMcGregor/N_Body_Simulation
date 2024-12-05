#ifndef BODY_H
#define BODY_H


#include "vector.h"
#include <string>

using namespace std;    //no more std::

class Body{
    protected:
        double radius;      //every body will have a radius
        Vector position;    //every body will have a position
        string type;        //type of body
    public:
        /*spot for constructor*/
    Body(double radius,const Vector& position, const string& type)
    : radius(radius),position(position),type(type){}
};

#endif