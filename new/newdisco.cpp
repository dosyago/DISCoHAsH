#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>

const uint64_t P = 1764231181088413613;
const uint64_t A = 1052833812310576281;

void newdisco_64(const void* key, int len, unsigned seed, void* out) {
    const uint8_t* data = reinterpret_cast<const uint8_t*>(key);

    uint64_t hash = seed;

    // Process each 64-bit chunk of the input data
    for (int i = 0; i < len / 8; ++i) {
        uint64_t k;
        memcpy(&k, data + i * 8, 8);

        // Core transformation
        k = (k * A) % P;

        // Mix the transformation into the accumulated hash value
        hash ^= k;
        hash = (hash << 1) | (hash >> 63); // Rotate left by 1
    }

    // Process remaining bytes if len is not a multiple of 8
    if (len % 8) {
        uint64_t k = 0;
        memcpy(&k, data + len / 8 * 8, len % 8);
        k = (k * A) % P;
        hash ^= k;
    }

    // Final mixing
    hash ^= len;
    hash ^= (hash >> 33);
    hash *= 0xff51afd7ed558ccd;
    hash ^= (hash >> 33);
    hash *= 0xc4ceb9fe1a85ec53;
    hash ^= (hash >> 33);

    // Copy the result to the output buffer
    memcpy(out, &hash, sizeof(hash));
}

