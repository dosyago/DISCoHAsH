#!/bin/bash

# Initial input - 32 bytes of zero
#input=$(printf '\0%.0s' {1..32})
input="so what ya doing there, son? huh?"

# Counter for the number of iterations
count=0

echo "Starting with 32 null bytes"

# Infinite loop
while true; do
    # Increment the counter
    ((count++))

    # Hash the input
    output=$(echo -n "$input" | ./bin/discosum)

    # Display the hash and count
    #echo "$count: $output"
    output="${output// /}"
    echo -n "$output"

    # Check if the first two bytes are 0x00
    if [[ "${output:0:8}" == "00000000" ]]; then

        echo "Found hash with first two bytes 0x00 after $count iterations."
        break
    fi

    # Set the output as the new input for the next iteration
    input=$output
done


