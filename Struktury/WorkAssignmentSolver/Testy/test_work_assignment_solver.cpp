#include "../WorkAssignmentSolver.h"

#include <cmath>
#include <iostream>
#include <vector>

bool nearDouble(double a, double b) {
    return std::fabs(a - b) < 0.000001;
}

bool testSimpleAssignment() {
    std::vector<Krasnal> dwarves = {
        Krasnal(1, Point(0, 0), {"zloto"}, "zloto", false)
    };
    std::vector<Kopalnia> mines = {
        Kopalnia(10, Point(3, 4), "zloto", 1, {})
    };

    WorkAssignmentSolver solver(dwarves, mines);
    WorkAssignmentResult result = solver.solve();

    return result.assignedCount == 1 &&
           result.preferredAssignedCount == 1 &&
           result.unassignedDwarfIds.empty() &&
           result.assignments.size() == 1 &&
           result.assignments[0].dwarfId == 1 &&
           result.assignments[0].mineId == 10 &&
           nearDouble(result.totalDistance, 5.0) &&
           solver.getMines()[0].getAssignedDwarves().size() == 1;
}

bool testPreferenceBeforeDistance() {
    std::vector<Krasnal> dwarves = {
        Krasnal(1, Point(0, 0), {"zloto", "miedz"}, "zloto", false)
    };
    std::vector<Kopalnia> mines = {
        Kopalnia(10, Point(10, 0), "zloto", 1, {}),
        Kopalnia(11, Point(1, 0), "miedz", 1, {})
    };

    WorkAssignmentSolver solver(dwarves, mines);
    WorkAssignmentResult result = solver.solve();

    return result.assignedCount == 1 &&
           result.preferredAssignedCount == 1 &&
           result.assignments.size() == 1 &&
           result.assignments[0].mineId == 10;
}

bool testMineCapacity() {
    std::vector<Krasnal> dwarves = {
        Krasnal(1, Point(0, 0), {"wegiel"}, "wegiel", false),
        Krasnal(2, Point(1, 0), {"wegiel"}, "wegiel", false)
    };
    std::vector<Kopalnia> mines = {
        Kopalnia(10, Point(0, 1), "wegiel", 2, {})
    };

    WorkAssignmentSolver solver(dwarves, mines);
    WorkAssignmentResult result = solver.solve();

    return result.assignedCount == 2 &&
           result.preferredAssignedCount == 2 &&
           result.unassignedDwarfIds.empty() &&
           solver.getMines()[0].getAssignedDwarves().size() == 2;
}

bool testUnassignedDwarf() {
    std::vector<Krasnal> dwarves = {
        Krasnal(1, Point(0, 0), {"wegiel"}, "wegiel", false)
    };
    std::vector<Kopalnia> mines = {
        Kopalnia(10, Point(0, 1), "zloto", 1, {})
    };

    WorkAssignmentSolver solver(dwarves, mines);
    WorkAssignmentResult result = solver.solve();

    return result.assignedCount == 0 &&
           result.preferredAssignedCount == 0 &&
           result.unassignedDwarfIds.size() == 1 &&
           result.unassignedDwarfIds[0] == 1 &&
           result.assignments.empty();
}

int main() {
    const bool test1 = testSimpleAssignment();
    const bool test2 = testPreferenceBeforeDistance();
    const bool test3 = testMineCapacity();
    const bool test4 = testUnassignedDwarf();

    std::cout << "Testy dla WorkAssignmentSolver:" << std::endl;
    std::cout << "Test 1: " << (test1 ? "OK" : "ERROR") << " (prosty przydzial)" << std::endl;
    std::cout << "Test 2: " << (test2 ? "OK" : "ERROR") << " (preferencja przed dystansem)" << std::endl;
    std::cout << "Test 3: " << (test3 ? "OK" : "ERROR") << " (pojemnosc kopalni)" << std::endl;
    std::cout << "Test 4: " << (test4 ? "OK" : "ERROR") << " (brak mozliwego przydzialu)" << std::endl;

    return (test1 && test2 && test3 && test4) ? 0 : 1;
}
