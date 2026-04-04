#ifndef PUNKT_H
#define PUNKT_H

struct Punkt {
    float x;
    float y;

    Punkt(float x, float y);

    bool operator<(const Punkt& other) const;
    bool operator>(const Punkt& other) const;
};

#endif
