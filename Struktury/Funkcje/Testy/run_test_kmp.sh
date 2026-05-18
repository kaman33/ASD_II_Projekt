#!/bin/bash

NAME="test_kmp.exe"

g++ -Wall -o $NAME test_kmp.cpp ../KMP.cpp
./$NAME
rm $NAME
