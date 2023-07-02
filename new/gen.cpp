#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include "math_utils.h"

// Helper function for fast modular exponentiation
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
    std::vector<uint64_t> factors = math_utils::factorize_source(p);
    uint64_t generator = math_utils::find_generator(factors, p);

    std::cout << "A generator modulo " << p << " is " << generator << std::endl;
    return 0;
}

