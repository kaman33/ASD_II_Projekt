#ifndef BORDER_PATROL_H
#define BORDER_PATROL_H

#include <vector>
#include "Point.h"

class BorderPatrolSolver {
private:
    std::vector<Point> activeMines;
    std::vector<Point> hull;
    bool calculated;

public:
    BorderPatrolSolver(const std::vector<Point> &mines);

    std::vector<Point> calculateConvexHull();
    double calculatePatrolDistance();

};

#endif