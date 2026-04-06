#!/bin/bash

NAME="test_is_point_on_segment.exe"

g++ -Wall -o $NAME test_is_point_on_segment.cpp ../../Point.cpp ../det.cpp
./$NAME
rm $NAME
