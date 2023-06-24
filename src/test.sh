#!/bin/bash

./build.sh

CHECK=$(./bin/bebbsum a2a647993898a3df12852766c497431fc4a5dae65da48cb7c4a5dae65da48cb7.txt)
VALUE="a2a647993898a3df12852766c497431fc4a5dae65da48cb7c4a5dae65da48cb7 a2a647993898a3df12852766c497431fc4a5dae65da48cb7c4a5dae65da48cb7.txt"
if [[ $CHECK == $VALUE ]]; then
  echo "Looks good."
else
  echo "Check value is wrong. Should be $VALUE"
fi

