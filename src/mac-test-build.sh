#!/bin/bash

rm -rf bin
mkdir bin
#-fopenmp unsupported
#g++ -g -march=native -Ofast *.cpp -o bin/discosum
clang++ -fsanitize=address -Wall -std=c++17 -march=native -O3 *.cpp -o bin/discosum

