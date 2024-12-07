#ifndef PLANET_H
#define PLANET_H

#include "Body.h"
#include <string>

using namespace std;

class Planet : public Body{
    public:
        Planet(double radius,double mass,Vector position, string type);
};

#endif