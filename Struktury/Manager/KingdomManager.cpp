#include "KingdomManager.h"

KingdomManager::KingdomManager()
    : inputPath("Dane/dane.txt"),
      solver(data.dwarves, data.mines),
      patrolSolver(activeMineLocations),
      guardSolver(data.guards) {}

void KingdomManager::resolvePath() {
    std::ifstream inputFile(inputPath);
    if (!inputFile.good()) {
        inputPath = "../Dane/dane.txt";
    }
}

void KingdomManager::processWorkAssignment() {
    data = loader.loadFromFile(inputPath);

    std::cout << "Krasnale: " << data.dwarves.size() << '\n';
    std::cout << "Kopalnie: " << data.mines.size() << '\n';
    std::cout << "Straznicy: " << data.guards.size() << '\n';
    
    solver = WorkAssignmentSolver(data.dwarves, data.mines);
    result = solver.solve();

    std::cout << "\nPrzydzial pracy:\n";
    for (const WorkAssignment& assignment : result.assignments) {
        std::cout << "Krasnal " << assignment.dwarfId
                  << " -> kopalnia " << assignment.mineId
                  << " (" << assignment.resourceType << "), dystans: "
                  << std::fixed << std::setprecision(2) << assignment.distance;

        if (assignment.preferredResource) {
            std::cout << " [preferowany surowiec]";
        }
        std::cout << '\n';
    }

    if (!result.unassignedDwarfIds.empty()) {
        std::cout << "\nBez przydzialu:";
        for (int dwarfId : result.unassignedDwarfIds) {
            std::cout << ' ' << dwarfId;
        }
        std::cout << '\n';
    }

    std::cout << "\nPrzydzielono: " << result.assignedCount << "/" << data.dwarves.size() << '\n';
    std::cout << "Przydzialy preferowane: " << result.preferredAssignedCount << '\n';
    std::cout << "Laczny dystans: " << std::fixed << std::setprecision(2) << result.totalDistance << '\n';
}

void KingdomManager::processBorderPatrol() {
    std::cout << "\nAktywne kopalnie:\n";
    for (const Kopalnia& mine : solver.getMines()) {
        if (!mine.isInUse()) {
            continue;
        }

        activeMineLocations.push_back(mine.getLocation());
        std::cout << "Kopalnia " << mine.getId()
                  << " (" << mine.getResourceType() << ") "
                  << mine.getLocation()
                  << ", krasnale:";

        for (int dwarfId : mine.getAssignedDwarves()) {
            std::cout << ' ' << dwarfId;
        }
        std::cout << '\n';
    }

    patrolSolver = BorderPatrolSolver(activeMineLocations);
    patrolHull = patrolSolver.calculateConvexHull();
    patrolDistance = patrolSolver.calculatePatrolDistance();

    std::cout << "\nPunkty trasy patrolu:\n";
    if (patrolHull.empty()) {
        std::cout << "Brak aktywnych kopalni.\n";
    } else {
        for (const Point& point : patrolHull) {
            std::cout << point << '\n';
        }
    }
    std::cout << "Dlugosc trasy patrolu: " << std::fixed << std::setprecision(2) << patrolDistance << '\n';
}

void KingdomManager::processGuardCommand() {
    guardSolver = GuardCommandSolver(data.guards);

    std::cout << "\nObrona granicy:\n";
    if (guardSolver.size() == 0) {
        std::cout << "Brak straznikow.\n";
    } else {
        GuardCommandResult fullRange = guardSolver.findLoudestGuard(0, guardSolver.size() - 1);
        if (fullRange.found) {
            std::cout << "Najglosniejszy na calej trasie: straznik "
                      << fullRange.guardId
                      << " na pozycji " << fullRange.index
                      << ", glosnosc: " << fullRange.loudness << '\n';
        }

        if (guardSolver.size() >= 3) {
            const int left = 1;
            const int right = std::min(3, guardSolver.size() - 1);
            GuardCommandResult attackedRange = guardSolver.findLoudestGuard(left, right);

            if (attackedRange.found) {
                std::cout << "Odcinek ataku [" << left << ", " << right << "]: straznik "
                          << attackedRange.guardId
                          << " na pozycji " << attackedRange.index
                          << ", glosnosc: " << attackedRange.loudness << '\n';
            }
        }
    }
}

void KingdomManager::saveToFile(const std::string& outputPath) {
    std::ofstream out(outputPath);
    
    out << "Przydzial pracy:\n";
    for (const WorkAssignment& assignment : result.assignments) {
        out << "Krasnal " << assignment.dwarfId
            << " -> kopalnia " << assignment.mineId
            << " (" << assignment.resourceType << "), dystans: "
            << std::fixed << std::setprecision(2) << assignment.distance;
        if (assignment.preferredResource) out << " [preferowany surowiec]";
        out << '\n';
    }
    out << "Laczny dystans: " << std::fixed << std::setprecision(2) << result.totalDistance << '\n';

    out << "\nTrasa patrolu:\n";
    for (const Point& point : patrolHull) {
        out << point << '\n';
    }
    out << "Dlugosc trasy patrolu: " << std::fixed << std::setprecision(2) << patrolDistance << '\n';

    out << "\nObrona granicy:\n";

    if (guardSolver.size() == 0) {
        out << "Brak straznikow.\n";
    } else {
        GuardCommandResult fullRange = guardSolver.findLoudestGuard(0, guardSolver.size() - 1);
        if (fullRange.found) {
            out << "Najglosniejszy na calej trasie: straznik "
                << fullRange.guardId
                << " na pozycji " << fullRange.index
                << ", glosnosc: " << fullRange.loudness << '\n';
        }

        if (guardSolver.size() >= 3) {
            const int left = 1;
            const int right = std::min(3, guardSolver.size() - 1);
            GuardCommandResult attackedRange = guardSolver.findLoudestGuard(left, right);
            if (attackedRange.found) {
                out << "Odcinek ataku [" << left << ", " << right << "]: straznik "
                    << attackedRange.guardId
                    << " na pozycji " << attackedRange.index
                    << ", glosnosc: " << attackedRange.loudness << '\n';
            }
        }
    }

    out.close();
}

void KingdomManager::visualize() {
    Visualizer visualizer(
        solver.getDwarves(),
        solver.getMines(),
        data.guards, 
        result, 
        patrolHull, 
        patrolDistance, 
        guardSolver
    );
    visualizer.run(); 
}

void KingdomManager::run() {
    resolvePath();
    processWorkAssignment();
    processBorderPatrol();
    processGuardCommand();
    saveToFile("output.txt");
}