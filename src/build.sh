#!/usr/bin/env bash

rm -rf bin
mkdir bin
if [[ "$(basename "$(readlink "$(which c++)")")" == clang++* ]]; then 
	clang++ -march=native -fopenmp -Ofast *.cpp -o bin/bebbsum
else
	c++ -march=native -fopenmp -Ofast *.cpp -o bin/bebbsum
fi
