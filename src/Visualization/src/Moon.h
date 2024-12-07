#ifndef MOON_H
#define MOON_H

#include "Body.h"
#include <string>

using namespace std;

class Moon : public Body{
    public:
        Moon(double radius,double mass, Vector position,string type);

};

#endif