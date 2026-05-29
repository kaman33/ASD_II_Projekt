#include "Parser/DataLoader.h"
#include "Struktury/WorkAssignmentSolver/WorkAssignmentSolver.h"
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

int main() {
    try {
        DataLoader loader;
        string inputPath = "Dane/dane.txt";
        ifstream inputFile(inputPath);
        if (!inputFile.good()) {
            inputPath = "../Dane/dane.txt";
        }

        InputData data = loader.loadFromFile(inputPath);

        cout << "Krasnale: " << data.dwarves.size() << '\n';
        cout << "Kopalnie: " << data.mines.size() << '\n';
        cout << "Straznicy: " << data.guards.size() << '\n';

        WorkAssignmentSolver solver(data.dwarves, data.mines);
        WorkAssignmentResult result = solver.solve();

        cout << "\nPrzydzial pracy:\n";
        for (const WorkAssignment& assignment : result.assignments) {
            cout << "Krasnal " << assignment.dwarfId
                 << " -> kopalnia " << assignment.mineId
                 << " (" << assignment.resourceType << "), dystans: "
                 << fixed << setprecision(2) << assignment.distance;

            if (assignment.preferredResource) {
                cout << " [preferowany surowiec]";
            }

            cout << '\n';
        }

        if (!result.unassignedDwarfIds.empty()) {
            cout << "\nBez przydzialu:";
            for (int dwarfId : result.unassignedDwarfIds) {
                cout << ' ' << dwarfId;
            }
            cout << '\n';
        }

        cout << "\nPrzydzielono: " << result.assignedCount << "/" << data.dwarves.size() << '\n';
        cout << "Przydzialy preferowane: " << result.preferredAssignedCount << '\n';
        cout << "Laczny dystans: " << fixed << setprecision(2) << result.totalDistance << '\n';
    }
    catch (const std::exception& e) {
        cerr << "Blad: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
