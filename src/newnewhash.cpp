#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "Platform.h"
#include "Hashlib.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>

constexpr int ROUNDS = 3;
constexpr uint64_t P         = UINT64_C(  0xFFFFFFFFFFFFFFFF) - 58;
constexpr uint64_t Q         = UINT64_C(13166748625691186689);
constexpr uint64_t R         = UINT64_C(1573836600196043749);
constexpr uint64_t S         = UINT64_C(1478582680485693857);
constexpr uint64_t T         = UINT64_C(1584163446043636637);
constexpr uint64_t U         = UINT64_C(1358537349836140151);
constexpr uint64_t V         = UINT64_C(2849285319520710901);
constexpr uint64_t W         = UINT64_C(2366157163652459183);
static const uint64_t K[8]      = {P, Q, R, S, T, U, V, W};
static const uint64_t Z[8]      = {17, 19, 23, 29, 31, 37, 41, 53}; // skip some, just because

constexpr uint64_t CTR_LEFT = UINT64_C(0xefcdab8967452301);
constexpr uint64_t CTR_RIGHT = UINT64_C(0x1032547698badcfe);

// Weak function that works with 512-bit (8x64-bit) blocks
void weakfunc(uint64_t* h, const uint64_t* data, bool left) {
  uint64_t ctr;
  if ( left ) {
    ctr = CTR_LEFT;
    for (int i = 0, j = 1, k = 8; i < 8; i++, j++, k++) {
      h[i] ^= data[i];            // ingest

      h[i] -= K[i];               // add 
      h[i] = ROTR64(h[i], Z[i]);  // rotate

      h[k] ^= h[i];               // xor blit high 512

      ctr += h[i];                // chain
      h[j] -= ctr;                // chain
    }
  } else {
    ctr = CTR_RIGHT;
    for (int i = 8, j = 0, k = 1; i < 16; i++, j++, k++) {
      h[i] ^= data[j];            // ingest

      h[i] -= K[j];               // add
      h[i] = ROTR64(h[i], Z[j]);  // rotate

      h[j] ^= h[i];               // blit low 512

      ctr += h[i];                // chain
      h[(k&7)+8] -= ctr;          // chain
    }
  }
}

template <uint32_t hashsize, bool bswap>
void newnewhash(const void* in, const size_t len, const seed_t seed, void* out) {
  const uint8_t * data = (const uint8_t *)in;
  uint64_t h[16] = {
      seed + len + 1,
      seed + len + 2,
      seed + len + 2,
      seed + len + 3,
      seed + len + 5,
      seed + len + 7,
      seed + len + 11,
      seed + len + 13,
      seed + len + 17,
      seed + len + 19,
      seed + len + 23,
      seed + len + 29,
      seed + len + 31,
      seed + len + 37,
      seed + len + 41,
      seed + len + 43
  };

  uint64_t temp[8];
  size_t lenRemaining = len;

  // Process 512-bit blocks
  while (lenRemaining >= 64) {
    for (int i = 0, j = 0; i < 8; ++i, j+= 8) {
      temp[i] = GET_U64<bswap>(data, j);
    }

    for( int i = 0; i < ROUNDS; i++) {
      weakfunc(h, temp, i&1);
    }

    data += 64;
    lenRemaining -= 64;
  }

  // Pad and process any remaining data less than 64 bytes (512 bits)
  memset(temp, (0x80+lenRemaining) & 255, sizeof(temp));
  memcpy(temp, data, lenRemaining);
  temp[lenRemaining / 8 + 1] = lenRemaining;

  for( int i = 0; i < ROUNDS; i++) {
    weakfunc(h, temp, i&1);
  }

  // Final processing
  for (int i = 0, j = 8; i < 8; i++, j++) {
    h[i] -= h[j];
  }

  // Output the hash
  for (uint32_t i = 0, j = 0; i < hashsize / 64; i++, j+= 8) {
    PUT_U64<bswap>(h[i], (uint8_t *)out, j);
  }
}

REGISTER_FAMILY(newnewhash,
   $.src_url  = "https://github.com/dosyago/discohash",
   $.src_status = HashFamilyInfo::SRC_STABLEISH
 );

REGISTER_HASH(NewNewhash,
   $.desc     = "New Newhash (aka TradHash)",
   $.impl_flags =
     FLAG_IMPL_MULTIPLY_64_64   |
     FLAG_IMPL_ROTATE       |
     FLAG_IMPL_SLOW       |
     FLAG_IMPL_LICENSE_MIT,
   $.bits = 64,
   $.verification_LE = 0x1D22D481,
   $.verification_BE = 0xC132DB16, 
   $.hashfn_native   = newnewhash<64, false>,
   $.hashfn_bswap  = newnewhash<64, true>
 );

REGISTER_HASH(NewNewhash_128,
   $.desc     = "Newhash (aka TradHash)",
   $.impl_flags =
     FLAG_IMPL_MULTIPLY_64_64   |
     FLAG_IMPL_ROTATE       |
     FLAG_IMPL_SLOW       |
     FLAG_IMPL_LICENSE_MIT,
   $.bits = 128,
   $.verification_LE = 0x20E47778,
   $.verification_BE = 0x4081DB5B,
   $.hashfn_native   = newnewhash<128, false>,
   $.hashfn_bswap  = newnewhash<128, true>
 );

