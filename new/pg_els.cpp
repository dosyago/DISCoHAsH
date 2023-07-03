#include <iostream>
#include <cstdlib>

int main( int argc, char **argv ) {
  uint64_t p; 
  uint64_t g; 

  if ( argc < 2 ) {
    std::cout << "Usage: " << argv[0] << " <prime> <generator>" << std::endl;
    p = 17;
    g = 3;
  } else {
    try {
      p = std::stoull(argv[1]);
    } catch(const std::invalid_argument& e) {
      std::cerr << "Prime argument is not a number" << std::endl;
      return 1; 
    } catch(const std::out_of_range& e) {
      std::cerr << "Prime argument out of range" << std::endl;
      return 1;
    }
    try {
      g = std::stoull(argv[2]);
    } catch(const std::invalid_argument& e) {
      std::cerr << "Generator argument is not a number" << std::endl;
      return 1; 
    } catch(const std::out_of_range& e) {
      std::cerr << "Generator argument out of range" << std::endl;
      return 1;
    }
  }

  uint64_t el = 1;
  for( int i = 0; i < p; i++ ) {
    std::cout << " g^k (mod p) == " << g << "^" << i << " (mod " << p << ") == " << el << std::endl; 
    el = (el * g) % p;
  }

  return 0;
}
