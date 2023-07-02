// math_utils.h

#pragma once
#include <string>
#include <cstdint>
#include <random>

namespace math_utils {
  int hamming_distance(uint64_t a, uint64_t b) {
      return std::bitset<64>(a ^ b).count();
  }

  inline std::vector<uint64_t> prime_factors(uint64_t n) {
      std::vector<uint64_t> factors;
      // Divide n by 2
      if (n % 2 == 0) {
          factors.push_back(2);
          while (n % 2 == 0) {
              n = n / 2;
          }
      }

      // Divide n by odd numbers starting from 3
      for (uint64_t i = 3; i * i <= n; i = i + 2) {
          if (n % i == 0) {
              factors.push_back(i);
              while (n % i == 0) {
                  n = n / i;
              }
          }
      }

      // If n is a prime number greater than 2
      if (n > 2) {
          factors.push_back(n);
      }

      return factors;
  }

  inline __uint128_t mod_pow(__uint128_t base, __uint128_t exponent, __uint128_t modulus) {
      __uint128_t result = 1;
      base %= modulus;
      while (exponent > 0) {
          if (exponent % 2 == 1) {
              result = (__uint128_t)(((__uint128_t)result * base) % modulus);
          }
          exponent >>= 1;
          base = (__uint128_t)(((__uint128_t)base * base) % modulus);
      }
      return result;
  }

  inline __uint128_t mod_inv(__uint128_t a, __uint128_t m) {
      __uint128_t m0 = m, t, q;
      __int128_t x0 = 0, x1 = 1;
      if (m == 1) return 0;
      while (a > 1) {
          q = a / m;
          t = m;
          m = a % m, a = t;
          t = x0;
          x0 = x1 - q * x0;
          x1 = t;
      }
      if (x1 < 0) x1 += m0;
      return x1;
  }

  inline bool miller_rabin_test(__uint128_t n, int k) {
      if (n <= 1 || (n > 2 && n % 2 == 0)) return false; // Base cases

      // Write n as d*2^r + 1
      __uint128_t d = n - 1;
      int r = 0;
      while (d % 2 == 0) {
          d /= 2;
          r++;
      }

      std::mt19937_64 rng(std::random_device{}());
      std::uniform_int_distribution<__uint128_t> dist(2, n - 2);

      // Repeat k times
      for (int i = 0; i < k; i++) {
          __uint128_t a = dist(rng);
          __uint128_t x = mod_pow(a, d, n);

          if (x == 1 || x == n - 1) continue;

          int j;
          for (j = 0; j < r - 1; j++) {
              x = mod_pow(x, 2, n); // Using modular_pow for squaring
              if (x == n - 1) break;
          }

          if (j == r - 1) return false; // n is composite
      }

      return true; // n is probably prime
  }

  std::vector<uint64_t> factorize_source(uint64_t p) {
      std::cout << "factoring " << p - 1 << " (p-1)..." << std::endl;

      std::vector<uint64_t> factors = prime_factors(p - 1); // Use prime_factors from math_utils
      
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
      std::uniform_int_distribution<uint64_t> dist(2, p - 2 /*floor(sqrt(p))*/);

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

  uint64_t find_first_generator(const std::vector<uint64_t>& factors, uint64_t p) {
      for (uint64_t g = 2; g < p; g++) {
          bool is_generator = true;
          for (uint64_t q : factors) {
              uint64_t dividend = (p - 1) / q;
              uint64_t congruence = mod_pow(g, dividend, p);

              if (congruence == 1) {
                  is_generator = false;
                  break;
              }
          }

          if (is_generator) {
              return g;
          }
      }

      throw std::runtime_error("Could not find a generator. Make sure p is a prime number greater than 2.");
  }

  uint64_t find_generator_balanced(uint64_t p) {
      const std::vector<uint64_t> factors = factorize_source(p);
      std::mt19937_64 rng(std::random_device{}());
      std::uniform_int_distribution<int> method_dist(0, 1); // 0 or 1 to choose the method

      if (method_dist(rng) == 0) {
          std::cout << "Using the random approach to find generator." << std::endl;
          return find_generator(factors, p);
      } else {
          std::cout << "Using the sequential approach to find generator." << std::endl;
          return find_first_generator(factors, p);
      }
  }


  inline std::string uint128_to_string(__uint128_t value) {
      std::string result;
      do {
          char digit = '0' + value % 10;
          result = digit + result;
          value /= 10;
      } while (value != 0);

      return result;
  }

  uint64_t find_prime() {
      std::mt19937_64 rng(std::random_device{}());
      std::uniform_int_distribution<uint64_t> dist(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max());
      uint64_t candidate;
      do {
          candidate = dist(rng);
      } while (!math_utils::miller_rabin_test(candidate, 20));
      return candidate;
  }

  inline uint64_t random_large_prime(int bits = 64) {
    return find_prime();
  }

  inline bool is_prime(uint64_t n) {
      return miller_rabin_test(n, 20);
  }
}



