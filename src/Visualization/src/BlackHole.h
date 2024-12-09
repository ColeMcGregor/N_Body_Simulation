#ifndef BLACKHOLE_H
#define BLACKHOLE_H

#include "Body.h"
#include <string>

using namespace std;

class BlackHole : public Body{
    public:
        BlackHole(double radius,Vector position, string type);
};

#endif