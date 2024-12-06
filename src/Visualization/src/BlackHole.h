#ifndef BLACKHOLE_H
#define BLACKHOLE_H

#include "Body.h"
#include <string>

using namespace std;

class BlackHole : public Body{
    private:
        double gravityMultiplier;

    public:
        BlackHole(double radius, double mass,Vector position, string type, double gravityMultiplier);
};

#endif