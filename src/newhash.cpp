#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "Hashlib.h"

static const uint64_t P         = UINT64_C(  0xFFFFFFFFFFFFFFFF) - 58;
static const uint64_t Q         = UINT64_C(13166748625691186689);
static const uint64_t R         = UINT64_C(1573836600196043749);
static const uint64_t S         = UINT64_C(1478582680485693857);
static const uint64_t T         = UINT64_C(1584163446043636637);
static const uint64_t U         = UINT64_C(1358537349836140151);
static const uint64_t V         = UINT64_C(2849285319520710901);
static const uint64_t W         = UINT64_C(2366157163652459183);

// unused
static const uint64_t G         = UINT64_C(1069112958779608227);
static const uint64_t H         = UINT64_C(1359858063359006325);

// unused
#define RL(value, rotate_bits) (((value) << (rotate_bits)) | ((value) >> ((64) - (rotate_bits))))

#define ROTR64(value, rotate_bits) (((value) >> (rotate_bits)) | ((value) << ((64) - (rotate_bits))))

static inline void mixA(uint64_t* s) {
  uint64_t a = s[0], b = s[1], c = s[2], d = s[3];

  a *= P;
  a = ROTR64(a, 23);
  a *= Q;

  b ^= a;

  b *= R;
  b = ROTR64(b, 29);
  b *= S;

  c *= T;
  c = ROTR64(c, 31);
  c *= U;

  d ^= c;

  d *= V;
  d = ROTR64(d, 37);
  d *= W;

  s[0] = a; s[1] = b; s[2] = c; s[3] = d;
}

static inline void mixB(uint64_t* s, uint64_t iv) {
  uint64_t a = s[1], b = s[2];

  a *= V;
  a = ROTR64(a, 23);
  a *= W;

  b ^= a + iv;

  b *= R;
  b = ROTR64(b, 23);
  b *= S;

  s[1] = a; s[2] = b; 
}

/** unused **/
static inline void mixC(uint64_t* s) {
  uint64_t a = s[0], b = s[1], c = s[2], d = s[3];

  const int k = 13;
  const int i = 64 - k;
  const int j = (a >> i);

  a = a << k | (b >> i);
  b = b << k | (c >> i);
  c = c << k | (d >> i);
  d = d << k | j;

  s[0] = a; s[1] = b; s[2] = c; s[3] = d;
}

template <uint32_t hashsize, bool bswap>
void newhash(const void* in, const size_t olen, const seed_t seed, void* out) {
  const uint8_t * data = (const uint8_t *)in;
  uint64_t h[4] = {seed + olen + 1, seed + olen + 3, seed + olen + 5, seed + olen + 7};
  size_t len = olen;
  uint64_t g = 0;
  bool inner = 0;

  while (len >= 16) {
    g =  GET_U64<bswap>(data, 0);

    h[0] -= g;
    h[1] += g;

    data += 8;

    g =  GET_U64<bswap>(data, 0);

    h[2] += g;
    h[3] -= g;

    if ( inner ) {
      mixB(h, seed);
    } else {
      mixA(h);
    }
    inner ^= 1;

    data += 8;
    len  -= 16;
  }

  mixB(h, seed);

  switch (len) {
    case 15:  h[0] += (uint64_t)data[14] << 56; 
    case 14:  h[1] += (uint64_t)data[13] << 48;
    case 13:  h[2] += (uint64_t)data[12] << 40; 
    case 12:  h[3] += (uint64_t)data[11] << 32;
    case 11:  h[0] += (uint64_t)data[10] << 24;
    case 10:  h[1] += (uint64_t)data[9] <<  16; 
    case 9:   h[2] += (uint64_t)data[8] << 8;
    case 8:   h[3] += data[7]; 
    case 7:   h[0] += (uint64_t)data[6] << 48; 
    case 6:   h[1] += (uint64_t)data[5] << 40;
    case 5:   h[2] += (uint64_t)data[4] << 32;
    case 4:   h[3] += (uint64_t)data[3] << 24;
    case 3:   h[0] += (uint64_t)data[2] << 16;
    case 2:   h[1] += (uint64_t)data[1] <<  8; 
    case 1:   h[2] += (uint64_t)data[0];
  }

  mixA(h);
  mixB(h, seed);
  mixA(h);

  g = 0;
  g -= h[2];
  g -= h[3];

  PUT_U64<bswap>(g, (uint8_t *)out, 0);
  if (hashsize == 128) {
    mixA(h);
    g = 0;
    g -= h[3];
    g -= h[2];
    PUT_U64<bswap>(g, (uint8_t *)out, 8);
  }
}

REGISTER_FAMILY(newhash,
   $.src_url  = "https://github.com/dosyago/discohash",
   $.src_status = HashFamilyInfo::SRC_STABLEISH
 );

REGISTER_HASH(Newhash,
   $.desc     = "Newhash (aka DiscoRainbow)",
   $.impl_flags =
     FLAG_IMPL_MULTIPLY_64_64   |
     FLAG_IMPL_ROTATE       |
     FLAG_IMPL_SLOW       |
     FLAG_IMPL_LICENSE_MIT,
   $.bits = 64,
   $.verification_LE = 0x2972C52E,
   $.verification_BE = 0x217A79E2,
   $.hashfn_native   = newhash<64, false>,
   $.hashfn_bswap  = newhash<64, true>
 );

REGISTER_HASH(Newhash_128,
   $.desc     = "Newhash (aka DiscoRainbow)",
   $.impl_flags =
     FLAG_IMPL_MULTIPLY_64_64   |
     FLAG_IMPL_ROTATE       |
     FLAG_IMPL_SLOW       |
     FLAG_IMPL_LICENSE_MIT,
   $.bits = 128,
   $.verification_LE = 0xFD9BCADD,
   $.verification_BE = 0xAF215EDF,
   $.hashfn_native   = newhash<128, false>,
   $.hashfn_bswap  = newhash<128, true>
 );
