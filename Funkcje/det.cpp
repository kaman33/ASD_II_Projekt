#include<vector>
#include "punkt.cpp"

float det(const Punkt p1, const Punkt p2, const Punkt p3){
    // 3 x 3 
    // D = (x2 − x1)(y3 − y1) − (y2 − y1)(x3 − x1)

    /*
        | 1 x1 y1 |  = p1
        | 1 x2 y2 |  = p2
        | 1 x3 y3 |  = p3

        D > 0 => p3 na lewo od p1p2
        D < 0 => p3 na prawo od p1p2
        D = 0 => p3 "na" p1p2 (wspolliniowy)
    
    */

    float D = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);

    return D;
}