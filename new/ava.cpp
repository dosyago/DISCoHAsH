#include <iostream>
#include <bitset>
#include <cstdint>

const uint64_t PRIME = 13166748625691186689ULL;
const uint64_t GENERATOR = 3180491373ULL;

uint64_t operation(uint64_t state, uint64_t message) {
    return (state + message) * GENERATOR % PRIME;
}

int main() {
    uint64_t original_state = 0x1234567890abcdef;
    uint64_t original_message = 0xfedcba9876543210;

    uint64_t original_output = operation(original_state, original_message);

    std::cout << "Bit changes in output due to single bit flip in input:\n";

    // For each bit in the state
    for (int i = 0; i < 64; i++) {
        uint64_t altered_state = original_state ^ (1ULL << i);
        uint64_t altered_output = operation(altered_state, original_message);
        uint64_t output_difference = original_output ^ altered_output;
        std::cout << "State bit " << i << ": " << std::bitset<64>(output_difference) << '\n';
    }

    // For each bit in the message
    for (int i = 0; i < 64; i++) {
        uint64_t altered_message = original_message ^ (1ULL << i);
        uint64_t altered_output = operation(original_state, altered_message);
        uint64_t output_difference = original_output ^ altered_output;
        std::cout << "Message bit " << i << ": " << std::bitset<64>(output_difference) << '\n';
    }

    return 0;
}

