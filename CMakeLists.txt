cmake_minimum_required(VERSION 3.31)
project(ASD_II_Projekt)

set(CMAKE_CXX_STANDARD 17)
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
        Struktury/GuardCommandSolver/GuardCommandSolver.h
        Struktury/GuardCommandSolver/GuardCommandSolver.cpp
        Struktury/Manager/KingdomManager.h
        Struktury/Manager/KingdomManager.cpp
        Struktury/SegmentTree.h
        Struktury/SegmentTree.cpp
        Struktury/Point.h
        Struktury/Point.cpp
        Struktury/Krasnal.h
        Struktury/Krasnal.cpp
        Struktury/Kopalnia.h
        Struktury/Kopalnia.cpp
        Struktury/Straznik.h
        Struktury/Straznik.cpp
        Struktury/Visualizer.h
        Struktury/Visualizer.cpp
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

        target_include_directories(ASD_II_Projekt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Struktury/SFML/include)
        target_link_directories(ASD_II_Projekt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Struktury/SFML/lib)
        target_link_libraries(ASD_II_Projekt PRIVATE sfml-graphics sfml-window sfml-system)

enable_testing()

get_filename_component(MINGW_RUNTIME_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)

# Automatyczne kopiowanie plików DLL z SFML do folderu binarnego po kompilacji
add_custom_command(TARGET ASD_II_Projekt POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/Struktury/SFML/bin/sfml-graphics-3.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/Struktury/SFML/bin/sfml-window-3.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/Struktury/SFML/bin/sfml-system-3.dll"
        "${MINGW_RUNTIME_DIR}/libgcc_s_seh-1.dll"
        "${MINGW_RUNTIME_DIR}/libstdc++-6.dll"
        "${MINGW_RUNTIME_DIR}/libwinpthread-1.dll"
        $<TARGET_FILE_DIR:ASD_II_Projekt>
    COMMENT "Kopiowanie bibliotek DLL SFML i runtime MinGW do folderu uruchomieniowego..."
)

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

add_executable(GuardCommandSolverTests
        Struktury/GuardCommandSolver/Testy/test_guard_command_solver.cpp
        Struktury/GuardCommandSolver/GuardCommandSolver.cpp
        Struktury/SegmentTree.cpp
        Struktury/Straznik.cpp)

add_test(NAME GuardCommandSolverTests COMMAND GuardCommandSolverTests)
