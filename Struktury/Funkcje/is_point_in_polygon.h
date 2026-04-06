#ifndef IS_POINT_IN_POLYGON_H
#define IS_POINT_IN_POLYGON_H

#include "../Point.h"
#include <vector>
#include "is_point_on_segment.h"
#include "do_segments_intersect.h"

bool is_point_in_polygon(Point p, const std::vector<Point>& polygon);

#endif