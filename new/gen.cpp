#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include "math_utils.h"

// Helper function for fast modular exponentiation
std::vector<uint64_t> factorize_source(uint64_t p) {
    std::cout << "factoring " << p - 1 << " (p-1)..." << std::endl;

    std::vector<uint64_t> factors = math_utils::prime_factors(p - 1); // Use prime_factors from math_utils
    
    std::cout << "factors of " << p - 1 << ": ";
    for (const auto& factor : factors) {
        std::cout << factor << " ";
    }
    std::cout << "\nfactoring complete." << std::endl;

    return factors;
}

uint64_t find_generator(const std::vector<uint64_t>& factors, uint64_t p) {
    std::cout << "Searching for a generator modulo " << p << "..." << std::endl;

    std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<uint32_t> dist(2, floor(sqrt(p)));

    uint64_t g;
    bool is_generator;
    while (true) {
        g = dist(rng);
        std::cout << "Trying candidate: " << g << std::endl;
        is_generator = true;

        for (uint64_t q : factors) {
            uint64_t dividend = (p - 1) / q;
            uint64_t congruence = math_utils::mod_pow(g, dividend, p);
            
            // Display the details of the test
            std::cout << "(" << g << ")^((" << p - 1 << ") / " << q << " = " << dividend << ") === " << congruence << " (mod " << p << ")" << std::endl;
            
            if (congruence == 1) {
                std::cout << "Candidate " << g << " failed." << std::endl;
                is_generator = false;
                break;
            }
        }

        if (is_generator) {
            std::cout << "Candidate " << g << " is a successful generator!" << std::endl;
            return g;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./program_name prime_number" << std::endl;
        return 1;
    }

    uint64_t p;
    try {
        p = std::stoull(argv[1]);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid input. Please enter a valid number." << std::endl;
        return 1;
    } catch (const std::out_of_range& e) {
        std::cerr << "Number out of range." << std::endl;
        return 1;
    }

    double logValue = log(p); // Computes the natural log of the number
    int log_ceil = static_cast<int>(ceil(logValue)); // Rounds up and converts to integer

    if ( ! math_utils::miller_rabin_test(p, log_ceil) ) {
      std::cerr << "The input number " << p << " is not a prime. Please enter a prime number." << std::endl;
      return 1; // Indicates that the program has encountered an error.
    }
    std::vector<uint64_t> factors = factorize_source(p);
    uint64_t generator = find_generator(factors, p);

    std::cout << "A generator modulo " << p << " is " << generator << std::endl;
    return 0;
}

