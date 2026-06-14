#ifndef KINGDOMMANAGER_H
#define KINGDOMMANAGER_H

#include "../../Parser/DataLoader.h"
#include "../../Struktury/BorderPatrolSolver.h"
#include "../../Struktury/GuardCommandSolver/GuardCommandSolver.h"
#include "../../Struktury/WorkAssignmentSolver/WorkAssignmentSolver.h"
#include "../../Struktury/Visualizer.h"
#include <algorithm>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

class KingdomManager {
private:
    std::string inputPath;
    DataLoader loader;
    InputData data;

    WorkAssignmentSolver solver;
    WorkAssignmentResult result;

    std::vector<Point> activeMineLocations;

    BorderPatrolSolver patrolSolver;
    std::vector<Point> patrolHull;
    double patrolDistance;

    GuardCommandSolver guardSolver;

    void resolvePath();
    void processWorkAssignment();
    void processBorderPatrol();
    void processGuardCommand();

public:
    KingdomManager();
    void run();
    void saveToFile(const std::string& outputPath = "output.txt");
    void visualize();
};

#endif