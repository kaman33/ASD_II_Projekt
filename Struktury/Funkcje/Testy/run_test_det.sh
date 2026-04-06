#!/bin/bash

NAME="test_det.exe"

g++ -Wall -o $NAME test_det.cpp ../det.cpp ../../Point.cpp
./$NAME
rm $NAME
