#include <vector>
#include "is_point_on_segment.cpp"

// Sprawdzenie czy odcinki p1p2 p3p4 się przeciniają
bool do_segments_intersect(Punkt p1, Punkt p2, Punkt p3, Punkt p4)
{
    float d1 = det(p1,p2,p3);
    float d2 = det(p1,p2,p4);
    float d3 = det(p3,p4,p1);
    float d4 = det(p3,p4,p2);

    // Przecinają się gdy
    // WARUNEK 2: każdy odcinek przecina prostą wyznaczoną przez drugi odcinek
    if((d1 * d2 < 0) && (d3 * d4 < 0)) return true;

    // WARUNEK 1: ktorys z koncow odc nalezy do drugiego odc
    if(is_point_on_segment(p3, p4, p1) || // Czy p1 leży na p3p4
        is_point_on_segment(p3, p4, p2) || // Czy p2 leży na p3p4
        is_point_on_segment(p1, p2, p3) || // Czy p3 leży na p1p2
        is_point_on_segment(p1, p2, p4)); // Czy p4 leży na p1p2
    
    return false;
}