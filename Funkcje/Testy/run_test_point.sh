#!/bin/bash

NAME="test_point.exe"

g++ -Wall -o $NAME test_point.cpp ../point.cpp
./$NAME
rm $NAME
