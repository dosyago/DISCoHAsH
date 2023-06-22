#!/bin/bash

rm -rf bin
mkdir bin
#-fopenmp unsupported
g++ -march=native -Ofast *.cpp -o bin/bebbsum
