#include <cmath>
#include "BorderPatrolSolver.h"
#include "Funkcje/graham_scan.h"
#include "Funkcje/distance.h"

BorderPatrolSolver::BorderPatrolSolver(const std::vector<Point> &mines)
    : activeMines(mines), calculated(false) {}

std::vector<Point> BorderPatrolSolver::calculateConvexHull() {
    if(!calculated){
        hull = graham_scan(activeMines);
        calculated = true;
    }
    return hull;
}

double BorderPatrolSolver::calculatePatrolDistance(){
    if(!calculated){
        hull = graham_scan(activeMines);
        calculated = true;
    }

    if(hull.size() < 2) return 0.0;

    double dist = 0.0;

    for(size_t i = 0; i < hull.size(); i++)
    {
        Point a = hull[i];
        Point b = hull[(i + 1) % hull.size()];
        dist += std::sqrt(distance(a,b)); // bo distance liczy kwadrat odleglosci
    }
    return dist;
}
