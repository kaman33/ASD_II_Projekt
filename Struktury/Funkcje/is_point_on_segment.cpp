#include "is_point_on_segment.h"
#include "EPS.h"

// Sprawdzenie czy punkt P należy do odcinka p1p2
bool is_point_on_segment(Point p1, Point p2, Point P)
{
    // P nalezy gdy p1 p2 i P sa współliniowe -> D = 0
    if(sgn(det(p1, p2, P)) != 0){
        return false;
    }
    // P nalezy gdy min(x1,x2) <= x3 <= max(x1,x2)
    // oraz max(y1, y2) <= y3 <= max(y1, y2)
    bool x_ok = cmpDouble(std::min(p1.x, p2.x), P.x) <= 0 &&
                cmpDouble(P.x, std::max(p1.x, p2.x)) <= 0;
    bool y_ok = cmpDouble(std::min(p1.y, p2.y), P.y) <= 0 &&
                cmpDouble(P.y, std::max(p1.y, p2.y)) <= 0;

    return x_ok && y_ok;
}

