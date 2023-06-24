#include <iostream>
#include <vector>
#include <fstream>
#include <inttypes.h>
#include <stdexcept>
#include <string>
#include <cstring>

#include "discohash.h"

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

void readStdinToBuffer(std::vector<uint8_t>& buffer) {
  std::istreambuf_iterator<char> begin(std::cin), end;
  std::vector<char> inputChars(begin, end);
  buffer = std::vector<uint8_t>(inputChars.begin(), inputChars.end());
}

int main(int argc, char **argv) {
  SET_BINARY_MODE(stdin);
  
  alignas(uint64_t)std::vector<uint8_t> buffer;
  std::string filename;
  bool infiniteMode = false;

  // Handle flags and arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--infinite") == 0) {
      SET_BINARY_MODE(stdout);
      infiniteMode = true;
    } else {
      filename = argv[i];
    }
  }

  bool readFromFile = !filename.empty() && filename != "-";
  if (readFromFile) {
    readFileToBuffer(filename, buffer);
  } else {
    readStdinToBuffer(buffer);
  }

  // Buffer to store the hash output
  std::vector<uint64_t> hash(4);

  // Sponge construction
  if (infiniteMode) {
    std::vector<uint8_t> input = buffer;
    while (true) {
      BEBB4185_64(input.data(), input.size(), 0, hash.data());
      std::fwrite(hash.data(), sizeof(uint64_t), 4, stdout);
      std::fflush(stdout); // make sure it's written
      // Reuse the same memory buffer as input for the next iteration
      std::memcpy(input.data(), hash.data(), sizeof(uint64_t) * 4);
    }
  } else {
    BEBB4185_64(buffer.data(), buffer.size(), 0, hash.data());
    for (int i = 0; i < 4; ++i) {
      printf("%016" PRIx64, hash[i]);
    }
    printf(" %s\n", filename.c_str());
  }

  return EXIT_SUCCESS;
}

