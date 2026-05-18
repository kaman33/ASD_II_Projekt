#!/bin/bash

NAME="test_min_cost_max_flow.exe"

g++ -Wall -o $NAME \
test_min_cost_max_flow.cpp \
../../Graph.cpp \
../../MinCostMaxFlow.cpp

./$NAME
rm $NAME
