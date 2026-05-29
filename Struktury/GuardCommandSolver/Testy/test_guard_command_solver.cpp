#include "../GuardCommandSolver.h"

#include <iostream>
#include <vector>

std::vector<Straznik> createGuards() {
    return {
        Straznik(100, 14),
        Straznik(101, 9),
        Straznik(102, 18),
        Straznik(103, 13),
        Straznik(104, 18),
        Straznik(105, 11)
    };
}

bool testWholeRange() {
    GuardCommandSolver solver(createGuards());
    GuardCommandResult result = solver.findLoudestGuard(0, solver.size() - 1);

    return result.found &&
           result.index == 2 &&
           result.guardId == 102 &&
           result.loudness == 18;
}

bool testSubRange() {
    GuardCommandSolver solver(createGuards());
    GuardCommandResult result = solver.findLoudestGuard(3, 5);

    return result.found &&
           result.index == 4 &&
           result.guardId == 104 &&
           result.loudness == 18;
}

bool testTieReturnsFirstGuard() {
    GuardCommandSolver solver({
        Straznik(1, 7),
        Straznik(2, 7),
        Straznik(3, 5)
    });

    GuardCommandResult result = solver.findLoudestGuard(0, 2);

    return result.found &&
           result.index == 0 &&
           result.guardId == 1 &&
           result.loudness == 7;
}

bool testUpdateLoudness() {
    GuardCommandSolver solver(createGuards());

    if (!solver.updateLoudness(1, 20)) {
        return false;
    }

    GuardCommandResult result = solver.findLoudestGuard(0, solver.size() - 1);

    return result.found &&
           result.index == 1 &&
           result.guardId == 101 &&
           result.loudness == 20;
}

bool testInvalidRange() {
    GuardCommandSolver solver(createGuards());
    GuardCommandResult result = solver.findLoudestGuard(4, 2);

    return !result.found;
}

bool testEmptyGuards() {
    GuardCommandSolver solver({});
    GuardCommandResult result = solver.findLoudestGuard(0, 0);

    return solver.size() == 0 && !result.found;
}

int main() {
    const bool test1 = testWholeRange();
    const bool test2 = testSubRange();
    const bool test3 = testTieReturnsFirstGuard();
    const bool test4 = testUpdateLoudness();
    const bool test5 = testInvalidRange();
    const bool test6 = testEmptyGuards();

    std::cout << "Testy dla GuardCommandSolver:" << std::endl;
    std::cout << "Test 1: " << (test1 ? "OK" : "ERROR") << " (caly zakres)" << std::endl;
    std::cout << "Test 2: " << (test2 ? "OK" : "ERROR") << " (podzakres)" << std::endl;
    std::cout << "Test 3: " << (test3 ? "OK" : "ERROR") << " (remis wybiera pierwszego)" << std::endl;
    std::cout << "Test 4: " << (test4 ? "OK" : "ERROR") << " (aktualizacja glosnosci)" << std::endl;
    std::cout << "Test 5: " << (test5 ? "OK" : "ERROR") << " (bledny zakres)" << std::endl;
    std::cout << "Test 6: " << (test6 ? "OK" : "ERROR") << " (brak straznikow)" << std::endl;

    return (test1 && test2 && test3 && test4 && test5 && test6) ? 0 : 1;
}
