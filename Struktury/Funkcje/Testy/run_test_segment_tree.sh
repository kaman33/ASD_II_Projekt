#!/bin/bash

NAME="test_segment_tree.exe"

g++ -Wall -o $NAME test_segment_tree.cpp ../../SegmentTree.cpp
./$NAME
rm $NAME