#ifndef STAR_H
#define STAR_H

#include "Body.h"
#include <string>

using namespace std;

class Star : public Body{
    private:
        double luminosity;      //luminosity of a star

    public:
        Star(double radius, Vector position, string type):
            Body(radius,position,type),luminosity(luminosity){}
};

#endif