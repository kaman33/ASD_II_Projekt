#ifndef PUNKT_H
#define PUNKT_H

#include <ostream>

struct Punkt {
    float x;
    float y;

    Punkt(float x, float y);

    bool operator<(const Punkt& other) const;
    bool operator>(const Punkt& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Punkt& p);
};

#endif
