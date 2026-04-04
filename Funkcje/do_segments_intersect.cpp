#include "do_segments_intersect.h"
#include "det.h"
#include "is_point_on_segment.h"

// Sprawdzenie czy odcinki p1p2 p3p4 się przeciniają
bool do_segments_intersect(Punkt p1, Punkt p2, Punkt p3, Punkt p4)
{
    float d1 = det(p1,p2,p3);
    float d2 = det(p1,p2,p4);
    float d3 = det(p3,p4,p1);
    float d4 = det(p3,p4,p2);

    // Przecinają się gdy:
    // WARUNEK 2: każdy odcinek przecina prostą wyznaczoną przez drugi odcinek
    if((d1 * d2 < 0) && (d3 * d4 < 0)) return true;

    // WARUNEK 1: ktorys z koncow odc. nalezy do drugiego odc.
    // Czy P1 leży na P3P4 || Czy P2 leży na P3P4 || Czy P3 leży na P1P2 || Czy P4 leży na P1P2
    if(is_point_on_segment(p3, p4, p1) || is_point_on_segment(p3, p4, p2) ||
        is_point_on_segment(p1, p2, p3) || is_point_on_segment(p1, p2, p4)) {
        return true;
    }
    return false;
}
