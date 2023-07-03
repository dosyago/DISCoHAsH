#include <iostream>
#include <bitset>
#include <cstdint>
#include <cstdlib>
#include <random>

uint64_t operation(uint64_t state, uint64_t message, uint64_t generator, uint64_t prime) {
    return (state + message) * generator % prime;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: program <PRIME> <GENERATOR>" << std::endl;
        return 1;
    }

    uint64_t PRIME = std::strtoull(argv[1], nullptr, 0);
    uint64_t GENERATOR = std::strtoull(argv[2], nullptr, 0);

    // Random number generation
    std::random_device rd;
    std::mt19937_64 eng(rd());
    std::uniform_int_distribution<uint64_t> distr;

    uint64_t original_state = distr(eng);
    uint64_t original_message = distr(eng);

    std::cout << "Original State: " << std::hex << original_state << std::endl;
    std::cout << "Original Message: " << std::hex << original_message << std::endl;

    uint64_t original_output = operation(original_state, original_message, GENERATOR, PRIME);

    std::cout << "Bit changes in output due to single bit flip in input:\n";

    // For each bit in the state
    for (int i = 0; i < 64; i++) {
        uint64_t altered_state = original_state ^ (1ULL << i);
        uint64_t altered_output = operation(altered_state, original_message, GENERATOR, PRIME);
        uint64_t output_difference = original_output ^ altered_output;
        std::cout << "State bit " << i << ": " << std::bitset<64>(output_difference) << '\n';
    }

    // For each bit in the message
    for (int i = 0; i < 64; i++) {
        uint64_t altered_message = original_message ^ (1ULL << i);
        uint64_t altered_output = operation(original_state, altered_message, GENERATOR, PRIME);
        uint64_t output_difference = original_output ^ altered_output;
        std::cout << "Message bit " << i << ": " << std::bitset<64>(output_difference) << '\n';
    }

    return 0;
}

