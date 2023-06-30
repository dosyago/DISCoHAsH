#include <iostream>
#include <vector>
#include "math_utils.h"

// Function to find generators of (Z/nZ)* where n = p*q
std::vector<uint64_t> find_generator_set(uint64_t p, uint64_t q) {
    uint64_t n = p * q;
    uint64_t phi = (p - 1) * (q - 1);
    
    // Factorize phi
    auto factors = math_utils::prime_factors(phi);
    
    std::vector<uint64_t> generators;

    // Loop through each element 'a' in (Z/nZ)* and check if it is a generator
    for (uint64_t a = 2; a < n; a++) {
        bool is_generator = true;
        for (uint64_t factor : factors) {
            // Check if a^(phi/factor) mod n is 1 for any factor of phi
            if (math_utils::mod_pow(a, phi / factor, n) == 1) {
                is_generator = false;
                break;
            }
        }
        // Additional check to ensure a^phi mod n is 1
        if (is_generator && math_utils::mod_pow(a, phi, n) != 1) {
            is_generator = false;
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
    std::cout << "\n\nAnd, guess what? Here are the orders of each generator:" << std::endl;
    
    uint64_t n = p * q;
    uint64_t phi = (p - 1) * (q - 1);
    
    for (auto& gen : generators) {
        uint64_t r = gen;
        uint64_t order = 1;
        // Magic optimization here!
        while (true) {
            r = (r * gen) % n;
            if (r == 1) break;
            order++;
        }
        
        std::cout << "The order of " << gen << " is: " << order << std::endl;
    } 

    std::cout << "Keep rocking the number theory! 🚀\n";
    return 0;
}

