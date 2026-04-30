#!/bin/bash

NAME="test_huffman.exe"

g++ -Wall -o $NAME test_huffman.cpp ../../Huffman.cpp
./$NAME
rm $NAME
