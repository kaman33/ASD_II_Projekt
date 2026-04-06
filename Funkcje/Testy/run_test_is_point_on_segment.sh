#!/bin/bash

NAME="test_is_point_on_segment.exe"

g++ -Wall -o $NAME test_is_point_on_segment.cpp ../point.cpp ../det.cpp
./$NAME
rm $NAME
