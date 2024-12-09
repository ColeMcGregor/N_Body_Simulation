#ifndef PLANET_H
#define PLANET_H

#include "Body.h"
#include <string>

using namespace std;

class Planet : public Body{
    public:
        Planet(double radius,Vector position, string type);
};

#endif