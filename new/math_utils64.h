// math_utils.h

#pragma once
#include <string>
#include <cstdint>
#include <random>

namespace math_utils {
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

  /*
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
  */

  
    inline uint64_t mod_pow(uint64_t base, uint64_t exponent, uint64_t modulus) {
        uint64_t result = 1;
        base %= modulus;
        while (exponent > 0) {
            if (exponent % 2 == 1) {
                result = (result * base) % modulus;
            }
            exponent >>= 1;
            base = (base * base) % modulus;
        }
        return result;
    }

    inline uint64_t mod_inv(uint64_t a, uint64_t m) {
        uint64_t m0 = m, t, q;
        int64_t x0 = 0, x1 = 1;
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

  inline std::string uint128_to_string(__uint128_t value) {
      std::string result;
      do {
          char digit = '0' + value % 10;
          result = digit + result;
          value /= 10;
      } while (value != 0);

      return result;
  }
}



