#ifndef PLANET_H
#define PLANET_H

#include "Body.h"
#include <string>

using namespace std;

class Planet : public Body{
    protected:
        //texture
        string name;//temp

    public:
        Planet(double radius,Vector position, string type):
            Body(radius,position,type),name(name){}

};

#endif