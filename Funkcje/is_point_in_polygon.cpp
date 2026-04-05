#include "is_point_in_polygon.h"

bool is_point_in_polygon(Point p, const std::vector<Point>& polygon){
    float u = polygon[0].x;
    float v = polygon[0].x;
    int n = polygon.size();
    for(int i = 0; i < n; i++){
        u = std::min(u, polygon[i].x);
        v = std::max(v, polygon[i].x);
    }

    if((p.x < u) || (p.x > v)) return false;

    int l = 0;

    Point q = Point(v + 1, p.y);

    for(int i = 0; i < n-1; i++){
        if(is_point_on_segment(polygon[i], polygon[i+1], p)) return true;
        if((p.y >= std::min(polygon[i].y, polygon[i+1].y)) && (p.y < std::max(polygon[i].y, polygon[i+1].y))){
            if(do_segments_intersect(polygon[i], polygon[i+1], p, q)) l++;
        }
    }

    if(l % 2 == 1) return true;
    return false;
}