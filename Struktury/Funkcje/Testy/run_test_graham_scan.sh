#!/bin/bash

NAME="test_graham_scan.exe"

g++ -Wall -o $NAME test_graham_scan.cpp ../graham_scan.cpp ../../Point.cpp ../det.cpp ../distance.cpp
./$NAME
rm $NAME
