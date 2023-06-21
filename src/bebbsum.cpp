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

    //size_t newSize = ((size + 7) / 8) * 8;  // Round up to nearest multiple of 8
    //buffer.resize(newSize, 0);  // Resize and fill with zeros
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

    DiscoHash hashObj;
    std::vector<uint8_t> hash(32);
    hashObj.BEBB4185_64(buffer.data(), buffer.size(), 0, hash.data());

    printf("%#018" PRIx64 "\n", *reinterpret_cast<uint64_t*>(hash.data()));
    std::cout << "Post Hash 2" << std::endl;

    return EXIT_SUCCESS;
}


