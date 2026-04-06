#!/bin/bash

NAME="test_do_segments_intersect.exe"

g++ -Wall -o $NAME test_do_segments_intersect.cpp ../../Point.cpp ../det.cpp ../is_point_on_segment.cpp
./$NAME
rm $NAME
