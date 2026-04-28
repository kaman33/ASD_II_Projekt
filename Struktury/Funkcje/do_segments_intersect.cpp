#include "do_segments_intersect.h"

// Sprawdzenie czy odcinki p1p2 p3p4 sie przecinaja
bool do_segments_intersect(Point p1, Point p2, Point p3, Point p4)
{
    double d1 = det(p1,p2,p3);
    double d2 = det(p1,p2,p4);
    double d3 = det(p3,p4,p1);
    double d4 = det(p3,p4,p2);

    // Przecinaja sie gdy:
    // WARUNEK 2: kazdy odcinek przecina prosta wyznaczona przez drugi odcinek
    if((sgn(d1) * sgn(d2) < 0) && (sgn(d3) * sgn(d4) < 0)) return true;

    // WARUNEK 1: ktorys z koncow odc. nalezy do drugiego odc.
    // Czy P1 lezy na P3P4 || Czy P2 lezy na P3P4 || Czy P3 lezy na P1P2 || Czy P4 lezy na P1P2
    if(is_point_on_segment(p3, p4, p1) || is_point_on_segment(p3, p4, p2) ||
        is_point_on_segment(p1, p2, p3) || is_point_on_segment(p1, p2, p4)) {
        return true;
    }
    return false;
}
