#ifndef PLANET_H
#define PLANET_H

#include "Body.h"
#include <string>

using namespace std;

class Planet : public Body{
    protected:
        //texture
        string material;    //rocky, gas

    public:
        Planet(double radius,double mass,Vector position, string type,string material);

};

#endif