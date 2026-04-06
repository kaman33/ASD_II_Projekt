#!/bin/bash

NAME="test_is_point_in_polygon.exe"

g++ -Wall -o $NAME test_is_point_in_polygon.cpp ../is_point_in_polygon.cpp ../../Point.cpp ../is_point_on_segment.cpp ../det.cpp ../do_segments_intersect.cpp
./$NAME
rm $NAME
