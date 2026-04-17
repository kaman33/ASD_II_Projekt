#include "../../BorderPatrolSolver.h"
#include "../EPS.h"
#include <iostream>

// Brak kopalni - odległość powinna być 0
bool test1() {
    std::vector<Point> mines = {};
    BorderPatrolSolver solver(mines);

    double dist = solver.calculatePatrolDistance();
    return cmpDouble(dist, 0.0) == 0.0;
}

// Jedna kopalnia - nie ma trasy do okrążenia
bool test2() {
    std::vector<Point> mines = {Point(1, 1)};
    BorderPatrolSolver solver(mines);

    double dist = solver.calculatePatrolDistance();
    return cmpDouble(dist, 0.0) == 0.0;
}

// Dwie kopalnie - trasa to odcinek tam i z powrotem
bool test3() {
    std::vector<Point> mines = {Point(0, 0), Point(3, 0)};
    BorderPatrolSolver solver(mines);

    double dist = solver.calculatePatrolDistance();
    return cmpDouble(dist, 6.0) == 0;
}

// Cztery kopalnie tworzą kwadrat 4x4 - obwód = 16
bool test4() {
    std::vector<Point> mines = {
        Point(0, 0), Point(4, 0), Point(4, 4), Point(0, 4)
    };
    BorderPatrolSolver solver(mines);

    double dist = solver.calculatePatrolDistance();
    return cmpDouble(dist, 16.0) == 0;
}

// Jeden punkt wewnątrz kwadratu - nie wpływa na otoczkę ani obwód
bool test5() {
    std::vector<Point> mines = {
        Point(0, 0), Point(4, 0), Point(4, 4), Point(0, 4), Point(2, 2)
    };
    BorderPatrolSolver solver(mines);

    double dist = solver.calculatePatrolDistance();
    return cmpDouble(dist, 16.0) == 0;
}

// Sprawdzamy czy calculateConvexHull zwraca właściwe punkty
bool test6() {
    std::vector<Point> mines = {
        Point(0, 0), Point(4, 0), Point(4, 4), Point(0, 4), Point(2, 2)
    };
    BorderPatrolSolver solver(mines);
    std::vector<Point> hull = solver.calculateConvexHull();

    // (2,2) nie powinien być w otoczce
    for(const Point& p : hull)
        if(p == Point(2, 2)) return false;
    return hull.size() == 4;
}

int main() {
    std::cout << "Testy dla BorderPatrolSolver:" << std::endl << std::endl;

    std::cout << "Test 1: " << (test1() ? "OK" : "ERROR") << " (pusty zbior)" << std::endl;
    std::cout << "Test 2: " << (test2() ? "OK" : "ERROR") << "(jedna kopalnia)" << std::endl;
    std::cout << "Test 3: " << (test3() ? "OK" : "ERROR") << "(dwie kopalnie)"<< std::endl;
    std::cout << "Test 4: " << (test4() ? "OK" : "ERROR") << "(kwadrat)" << std::endl;
    std::cout << "Test 5: " << (test5() ? "OK" : "ERROR") << "(punkt wewnatrz)" << std::endl;
    std::cout << "Test 6: " << (test6() ? "OK" : "ERROR") << "(poprawnosc otoczki)" << std::endl;

    return 0;
}