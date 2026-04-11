#ifndef EPS_H
#define EPS_H

constexpr double EPS = 1e-9;

inline int cmpDouble(double a, double b) {
    if (a < b - EPS) return -1;
    if (a > b + EPS) return 1;
    return 0;
}

inline int sgn(double x) {
    return cmpDouble(x, 0.0);
}

#endif
