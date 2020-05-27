#!/bin/bash

./build.sh
CHECK=$(./bin/bebbsum *.txt)
VALUE=$(basename *.txt .txt)
if [[ $CHECK -eq $VALUE ]]; then
  echo "Looks good."
else
  echo "Check value is wrong. Should be $VALUE"
fi

