#include <iostream>
#include <vector>
#include <cstdlib>
#include "math_utils.h"

int main(int argc, char* argv[]) {
    using namespace math_utils;

    if (argc != 5) {
        std::cerr << "Usage: ./program P Q A B" << std::endl;
        return 1;
    }

    uint64_t P = std::strtoull(argv[1], nullptr, 10);
    uint64_t Q = std::strtoull(argv[2], nullptr, 10);
    uint64_t A = std::strtoull(argv[3], nullptr, 10);
    uint64_t B = std::strtoull(argv[4], nullptr, 10);

    // Calculate modular inverses
    __uint128_t A_inv_mod_Q = mod_inv(A, Q);
    __uint128_t B_inv_mod_P = mod_inv(B, P);

    // Print the modular inverses
    std::cout << "A^-1 mod Q = " << uint128_to_string(A_inv_mod_Q) << std::endl;
    std::cout << "B^-1 mod P = " << uint128_to_string(B_inv_mod_P) << std::endl;

    // Initialize E and F
    __uint128_t E = 1;
    __uint128_t F = 1;
    __uint128_t prev_E = E;
    __uint128_t prev_F = F;

    char cont = 'y';

    // Calculate E_i+1 and F_i+1 and print them along with Hamming distances
    for (uint64_t i = 0; i < P && (cont == 'y' || cont == 'Y'); i++) {
        prev_E = E;
        prev_F = F;
        E = ((__uint128_t)((E * A) % P) * A_inv_mod_Q) % Q;
        F = ((__uint128_t)((((((F * A) % P) * B) % Q) * B_inv_mod_P) * A_inv_mod_Q) % Q) % P;
        
        std::cout << "E_" << i+1 << " = " << uint128_to_string(E) << ", "
                  << "F_" << i+1 << " = " << uint128_to_string(F) << ", "
                  << "H(E_" << i+1 << ", E_" << i << ") = " << hamming_distance(static_cast<uint64_t>(E), static_cast<uint64_t>(prev_E)) << ", "
                  << "H(F_" << i+1 << ", F_" << i << ") = " << hamming_distance(static_cast<uint64_t>(F), static_cast<uint64_t>(prev_F)) << std::endl;

        std::cout << "Continue? (y/n): ";
        std::cin >> cont;
    }

    return 0;
}

