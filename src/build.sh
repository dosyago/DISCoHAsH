#!/bin/bash

rm -rf bin
mkdir bin
#-fopenmp unsupported
g++ -march=native -Ofast *.cpp -o bin/bebbsum
#clang++ -fsanitize=address -Wall -gp -std=c++17 -march=native -O3 *.cpp -o bin/bebbsum

