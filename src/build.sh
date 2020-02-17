#!/bin/bash

rm -rf bin
mkdir bin
g++ -march=native -fopenmp -Ofast *.cpp -o bin/bebbsum
