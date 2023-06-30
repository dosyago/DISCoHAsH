#include <iostream>
#include <cmath>
#include "math_utils.h"

uint64_t crt_element(uint64_t x, uint64_t y, uint64_t P, uint64_t Q, uint64_t A, uint64_t B, uint64_t invP, uint64_t invQ) {
    uint64_t term1 = (math_utils::mod_pow(A, x, P) * Q * invQ) % (P * Q);
    uint64_t term2 = (math_utils::mod_pow(B, y, Q) * P * invP) % (P * Q);
    uint64_t element = (term1 + term2) % (P * Q);
    return element;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: ./program_name P Q A B" << std::endl;
        return 1;
    }

    uint64_t P = std::stoull(argv[1]);
    uint64_t Q = std::stoull(argv[2]);
    uint64_t A = std::stoull(argv[3]);
    uint64_t B = std::stoull(argv[4]);

    // Check if P and Q are primes
    if (!math_utils::miller_rabin_test(P, 5) || !math_utils::miller_rabin_test(Q, 5)) {
        std::cerr << "Both P and Q should be prime numbers." << std::endl;
        return 1;
    }

    // Compute modular inverses
    int64_t invP = math_utils::mod_inv(Q, P);
    int64_t invQ = math_utils::mod_inv(P, Q);

    // Check if the modular inverses exist
    if (invP == -1 || invQ == -1) {
        std::cerr << "Modular inverses do not exist for the given P and Q." << std::endl;
        return 1;
    }

    // Loop through elements of the groups modulo P and Q
    for (uint64_t x = 1; x < P; x++) {
        for (uint64_t y = 1; y < Q; y++) {
            uint64_t element = crt_element(x, y, P, Q, A, B, invP, invQ);
            std::cout << "Element: " << element << " (x=" << x << ", y=" << y << ")" << std::endl;
        }
    }

    return 0;
}

