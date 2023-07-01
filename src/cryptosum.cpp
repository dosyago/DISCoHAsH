#include <iostream>
#include <vector>
#include <fstream>
#include <inttypes.h>
#include <stdexcept>

#include "discohash.h"

// Check if we are on win or linux to handle the binary mode
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#define SET_BINARY_MODE(handle) _setmode(_fileno(handle), _O_BINARY)
#else
#define SET_BINARY_MODE(handle) ((void)0)
#endif

void readFileToBuffer(const std::string& filename, std::vector<uint8_t>& buffer) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (file.fail()) {
        throw std::runtime_error("Unable to open the file: " + filename);
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer.resize(size, 0);

    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Failed to read the file: " + filename);
    }
}

int main(int argc, char **argv) {
  SET_BINARY_MODE(stdin);
  std::vector<uint8_t> buffer;
  std::string filename;

  if (argc > 1) {
      filename = argv[1];
      if(filename != "-") {
          readFileToBuffer(filename, buffer);
      } else {
          std::istreambuf_iterator<char> begin(std::cin), end;
          std::vector<char> inputChars(begin, end);
          buffer = std::vector<uint8_t>(inputChars.begin(), inputChars.end());
      }
  } else {
      std::istreambuf_iterator<char> begin(std::cin), end;
      std::vector<char> inputChars(begin, end);
      buffer = std::vector<uint8_t>(inputChars.begin(), inputChars.end());
  }

  std::vector<uint64_t> hash(4);
  DISCoHAsH_512_64(buffer.data(), buffer.size(), 0, hash.data());

  for (int i = 0; i < 4; ++i) {
    printf("%016" PRIx64, hash[i]);
  }

  printf(" %s\n", filename.c_str());

  return EXIT_SUCCESS;
}

