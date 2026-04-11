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
        Struktury/Funkcje/graham_scan.h
        Struktury/Funkcje/graham_scan.cpp
        Struktury/Funkcje/is_point_on_segment.h
        Struktury/Funkcje/is_point_on_segment.cpp
        Struktury/Funkcje/do_segments_intersect.h
        Struktury/Funkcje/do_segments_intersect.cpp
        Struktury/Funkcje/is_point_in_polygon.h
        Struktury/Funkcje/is_point_in_polygon.cpp
        Struktury/Funkcje/EPS.h)
