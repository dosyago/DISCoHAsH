#!/bin/bash

# Check if script and watch are installed
if ! command -v script &> /dev/null; then
    echo "script could not be found. Please ensure it is available before running this script."
    exit 1
fi

if ! command -v watch &> /dev/null; then
    echo "watch could not be found. Please install it before running this script."
    exit 1
fi

# Check if the script is called with the required parameter
if [ "$#" -ne 1 ]; then
    echo "Usage: ./script_name <hashName>"
    exit 1
fi

tests=(
    Sanity
    Speed
    Hashmap
    Avalanche
    Sparse
    Permutation
    Window
    Cyclic
    TwoBytes
    Text
    Zeroes
    Seed
    PerlinNoise
    Diff
    DiffDist
    BIC
    MomentChi2
    Prng
    BadSeeds
)

hashName="$1"
# Getting current datetime in ISO 8601 format with minute precision
# Note: We can use an alternative format string on macOS:
datetime=$(date +"%Y-%m-%dT%H:%M")

pipe="pipe_$RANDOM"

# Create a named pipe for live output
mkfifo "$pipe"

for test in "${tests[@]}"; do
    # Use script -q -F to mimic unbuffer (macOS syntax)
    script -q -F "$pipe" ./SMHasher --test="$test" "$hashName" > "${test}_t_${hashName}_${datetime}.results.txt" 2>&1 &
done

# Sleep for a short duration to allow the processes to start
sleep 2

# Watching for output containing '!!!!'
watch grep '!!!!' *t_${hashName}_${datetime}.results.txt

# Clean up the named pipe
rm "$pipe"

