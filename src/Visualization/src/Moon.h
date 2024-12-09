#ifndef MOON_H
#define MOON_H

#include "Body.h"
#include <string>

using namespace std;

class Moon : public Body{
    public:
        Moon(double radius, Vector position,string type);

};

#endif