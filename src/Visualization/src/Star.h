#ifndef STAR_H
#define STAR_H

#include "Body.h"
#include <string>

using namespace std;
class Star : public Body {
    private:
        double luminosity; // Luminosity of the star

    public:
        // Constructor
        Star(double radius, double mass, const Vector& position, const string& type, double luminosity);

        // Accessor for luminosity
        double getLuminosity() const { return luminosity; }
};

#endif