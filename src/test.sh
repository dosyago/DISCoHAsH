#!/bin/bash

./build.sh

./bin/bebbsum 32.txt
./bin/bebbsum 33.txt


#CHECK=$(./bin/bebbsum *.txt)
#VALUE=$(basename *.txt .txt)
#if [[ $CHECK -eq $VALUE ]]; then
#  echo "Looks good."
#else
#  echo "Check value is wrong. Should be $VALUE"
#fi

