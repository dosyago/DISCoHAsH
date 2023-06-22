#!/bin/bash

./build.sh

CHECK=$(./bin/bebbsum 0x*.txt)
VALUE=$(basename 0x*.txt .txt)
if [[ $CHECK -eq $VALUE ]]; then
  echo "Looks good."
else
  echo "Check value is wrong. Should be $VALUE"
fi

