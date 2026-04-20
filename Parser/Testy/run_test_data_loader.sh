#!/bin/bash

NAME="test_data_loader.exe"

g++ -Wall -std=c++14 -o "$NAME" \
test_data_loader.cpp \
../DataLoader.cpp \
../../Struktury/Krasnal.cpp \
../../Struktury/Kopalnia.cpp \
../../Struktury/Straznik.cpp \
../../Struktury/Point.cpp

if [ $? -ne 0 ]; then
    exit 1
fi

./"$NAME"
STATUS=$?
rm -f "$NAME"
exit $STATUS
