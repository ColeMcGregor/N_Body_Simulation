#ifndef STAR_H
#define STAR_H

#include "Body.h"
#include <string>

using namespace std;
class Star : public Body {
    public:
        // Constructor
        Star(double radius, double mass, const Vector& position, const string& type);
};

#endif
