#!/bin/bash

rm -rf bin
mkdir bin
g++ -march=native -Ofast *.cpp -o bin/bebbsum
