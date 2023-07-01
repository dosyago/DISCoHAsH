#include <iostream>
#include <cstdint>
#include "math_utils.h"

int main() {
    // Constants
    const uint64_t P = 18446744073709551557ULL;
    const uint64_t Q = 13166748625691186689ULL;

    // Compute modular inverses
    uint64_t invQ_PQ = math_utils::mod_inv(Q, P);
    uint64_t invP_PQ = math_utils::mod_inv(P, Q);

    // Print the modular inverses
    std::cout << "invQ_PQ: " << invQ_PQ << std::endl;
    std::cout << "invP_PQ: " << invP_PQ << std::endl;

    return 0;
}

