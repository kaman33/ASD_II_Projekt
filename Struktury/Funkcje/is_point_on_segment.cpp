#include "is_point_on_segment.h"

// Sprawdzenie czy punkt P należy do odcinka p1p2
bool is_point_on_segment(Point p1, Point p2, Point P)
{
    // P nalezy gdy p1 p2 i P sa współliniowe -> D = 0
    if(det(p1, p2, P) != 0){
        return false;
    }
    // P nalezy gdy min(x1,x2) <= x3 <= max(x1,x2)
    // oraz max(y1, y2) <= y3 <= max(y1, y2)
    bool x_ok = (std::min(p1.x, p2.x) <= P.x) && (P.x <= std::max(p1.x, p2.x));
    bool y_ok = (std::min(p1.y, p2.y) <= P.y && (P.y <= std::max(p1.y, p2.y)));

    return x_ok && y_ok;
}

