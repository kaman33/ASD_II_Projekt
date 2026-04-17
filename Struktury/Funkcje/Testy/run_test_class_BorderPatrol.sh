#!/bin/bash

NAME="test_class_BorderPatrol.exe"

g++ -Wall -o $NAME \
test_class_BorderPatrol.cpp \
../../BorderPatrolSolver.cpp \
../../Funkcje/graham_scan.cpp \
../../Funkcje/det.cpp \
../../Funkcje/distance.cpp \
../../Point.cpp

./$NAME
rm $NAME