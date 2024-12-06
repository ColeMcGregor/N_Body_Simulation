#ifndef MOON_H
#define MOON_H

#include "Body.h"
#include <string>

using namespace std;

class Moon : public Body{
    private:
        int planetID;       //id of the planet that it follows

    public:
        Moon(double radius,double mass, Vector position,string type, int planetID);

};

#endif