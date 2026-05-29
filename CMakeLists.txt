cmake_minimum_required(VERSION 3.31)
project(ASD_II_Projekt)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ASD_II_Projekt
        main.cpp
        Parser/InputData.h
        Parser/DataLoader.h
        Parser/DataLoader.cpp
        Struktury/BorderPatrolSolver.h
        Struktury/BorderPatrolSolver.cpp
        Struktury/Graph.h
        Struktury/Graph.cpp
        Struktury/MinCostMaxFlow.h
        Struktury/MinCostMaxFlow.cpp
        Struktury/WorkAssignmentSolver/WorkAssignmentSolver.h
        Struktury/WorkAssignmentSolver/WorkAssignmentSolver.cpp
        Struktury/Point.h
        Struktury/Point.cpp
        Struktury/Krasnal.h
        Struktury/Krasnal.cpp
        Struktury/Kopalnia.h
        Struktury/Kopalnia.cpp
        Struktury/Straznik.h
        Struktury/Straznik.cpp
        Struktury/Funkcje/det.h
        Struktury/Funkcje/det.cpp
        Struktury/Funkcje/distance.h
        Struktury/Funkcje/distance.cpp
        Struktury/Funkcje/graham_scan.h
        Struktury/Funkcje/graham_scan.cpp
        Struktury/Funkcje/is_point_on_segment.h
        Struktury/Funkcje/is_point_on_segment.cpp
        Struktury/Funkcje/do_segments_intersect.h
        Struktury/Funkcje/do_segments_intersect.cpp
        Struktury/Funkcje/is_point_in_polygon.h
        Struktury/Funkcje/is_point_in_polygon.cpp
        Struktury/Funkcje/EPS.h)

enable_testing()

add_executable(DataLoaderTests
        Parser/Testy/test_data_loader.cpp
        Parser/DataLoader.cpp
        Struktury/Point.cpp
        Struktury/Krasnal.cpp
        Struktury/Kopalnia.cpp
        Struktury/Straznik.cpp)

add_test(NAME DataLoaderTests COMMAND DataLoaderTests)

add_executable(MinCostMaxFlowTests
        Struktury/Funkcje/Testy/test_min_cost_max_flow.cpp
        Struktury/Graph.cpp
        Struktury/MinCostMaxFlow.cpp)

add_test(NAME MinCostMaxFlowTests COMMAND MinCostMaxFlowTests)

add_executable(BorderPatrolSolverTests
        Struktury/Funkcje/Testy/test_class_BorderPatrol.cpp
        Struktury/BorderPatrolSolver.cpp
        Struktury/Funkcje/graham_scan.cpp
        Struktury/Funkcje/det.cpp
        Struktury/Funkcje/distance.cpp
        Struktury/Point.cpp)

add_test(NAME BorderPatrolSolverTests COMMAND BorderPatrolSolverTests)

add_executable(WorkAssignmentSolverTests
        Struktury/WorkAssignmentSolver/Testy/test_work_assignment_solver.cpp
        Struktury/WorkAssignmentSolver/WorkAssignmentSolver.cpp
        Struktury/Graph.cpp
        Struktury/MinCostMaxFlow.cpp
        Struktury/Point.cpp
        Struktury/Krasnal.cpp
        Struktury/Kopalnia.cpp
        Struktury/Funkcje/distance.cpp)

add_test(NAME WorkAssignmentSolverTests COMMAND WorkAssignmentSolverTests)
