#!/bin/bash

rm -rf bin
mkdir bin
#-fopenmp unsupported
#g++ -march=native -Ofast *.cpp -o bin/discosum
#clang++ -fsanitize=address,undefined -Wall -g -std=c++17 -march=native -O3 *.cpp -o bin/discosum
clang++ -std=c++17 -march=native -O3 *.cpp -o bin/discosum

