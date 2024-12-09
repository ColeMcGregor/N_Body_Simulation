#ifndef STAR_H
#define STAR_H

#include "Body.h"
#include <string>

using namespace std;
class Star : public Body {
    public:
        // Constructor
        //Star(double radius, const Vector& position, const string& type);
        Star(int id, const string& type, double radius);
};

#endif
