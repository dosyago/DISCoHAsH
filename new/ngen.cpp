#include <iostream>
#include <vector>
#include "math_utils.h"

// Function to find generators of (Z/nZ)* where n = p*q
std::vector<uint64_t> find_generator_set(uint64_t p, uint64_t q) {
    uint64_t n = p * q;
    uint64_t phi = (p - 1) * (q - 1);
    
    // Factorize phi
    auto factors = math_utils::prime_factors(phi);
    std::cout << "Prime factors:";
    for (uint64_t factor : factors) {
        std::cout << " " << factor;
    }
    std::cout << std::endl;
    
    std::vector<uint64_t> generators;

    for (uint64_t a = 2; a < n; a++) {
        bool is_generator = true;
        for (uint64_t factor : factors) {
            uint64_t exp = phi / factor;
            uint64_t result = math_utils::mod_pow(a, exp, n);
            //std::cout << "Checking " << a << "^(phi/" << factor << ") = " << a << "^(" << exp << ") mod " << n << " = " << result << std::endl;  // Debug output
            if (result == 1) {
                is_generator = false;
                break;
            }
        }
        if (is_generator) {
            generators.push_back(a);
        }
    }

    
    return generators;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./generator_set p q" << std::endl;
        return 1;
    }

    uint64_t p = std::stoull(argv[1]);
    uint64_t q = std::stoull(argv[2]);

    auto generators = find_generator_set(p, q);
    
    std::cout << "\nBravo! 🎉 The set of generators of (Z/" << p * q << "Z)* is: ";
    for (auto& gen : generators) {
        std::cout << gen << " ";
    }
    std::cout << std::endl;

    __uint128_t n = p*q;
    __uint128_t phi = (p - 1) * (q - 1);
    
    std::cout << "The order of (Z/" << math_utils::uint128_to_string(n) << "Z)* is: " << math_utils::uint128_to_string(phi) << std::endl;
    std::cout << "\n\nAnd, guess what? Here are the orders of each generator:" << std::endl;
    
    for (auto& gen : generators) {
        uint64_t order = 1;
        uint64_t value = gen;
        while (true) {
            value = (value * gen) % n;
            order++;
            // Check if it cycles back to itself
            if (value == gen) {
                break;
            }
        }
        std::cout << "The order of " << gen << " is: " << order << std::endl;
    }

    std::cout << "Keep rocking the number theory! 🚀\n";
    return 0;
}

