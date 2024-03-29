// also known as BEBB4185
// Copyright 2020 Cris Stringfellow
// https://github.com/dosyago/discohash
#include <cstdio>
#include <inttypes.h>
#include <cstring>
#include <iostream>
#include "discohash.h"

constexpr int STATE = 64;
constexpr int STATEM = STATE-1;
constexpr int STATE64 = STATE >> 3;
constexpr int HSTATE64 = STATE64 >> 1;
constexpr int STATE64M = STATE64-1;
constexpr int HSTATE64M = HSTATE64-1;
alignas(uint64_t) uint8_t disco_c_buf[STATE] = {0};
constexpr uint64_t P = 0xFFFFFFFFFFFFFFFF - 58;
constexpr uint64_t Q = 13166748625691186689U;
alignas(uint64_t) uint8_t *dcs8 = (uint8_t *)disco_c_buf;
uint64_t *dcs = (uint64_t *)disco_c_buf;

  static inline uint64_t rot( uint64_t v, int n) 
  {
    n = n & 63U;
    if (n)
        v = (v >> n) | (v << (64-n));
    return v; 
  }

  static inline uint8_t rot8( uint8_t v, int n) 
  {
    n = n & 7U;
    if (n)
        v = (v >> n) | (v << (8-n));
    return v; 
  }

  static inline void mix(const int A)
  {
    const int B = A+1;
    dcs[A] *= P;
    dcs[A] = rot(dcs[A], 23);
    dcs[A] *= Q;
    
    dcs[B] ^= dcs[A];

    dcs[B] *= P;
    dcs[B] = rot(dcs[B], 23);
    dcs[B] *= Q;
  }

  static inline void round( const uint64_t * m64, const uint8_t * m8, int len )
  {
    int index = 0;
    int sindex = 0;
    uint64_t counter = 0xfaccadaccad09997;
    uint8_t counter8 = 137;

    for( int Len = len >> 3; index < Len; index++) {
      dcs[sindex] += rot(m64[index] + index + counter + 1, 23);
      counter += ~m64[index] + 1;
      switch(sindex) {
        case 1:
          mix(0);
          sindex++;
          break;
        case 3:
          mix(2);
          sindex++;
          break;
        case 5: 
          mix(4);
          sindex++;
          break;
        case 7:
          mix(6);
          sindex = 0;
          break;
        default:
          sindex++;
          break;
      }
    }

    mix(1);
    mix(3);
    mix(5);


    index <<= 3;
    sindex = index&(STATEM);
    for( ; index < len; index++) {
      dcs8[sindex] += rot8(m8[index] + index + counter8 + 1, 23);
      counter8 += ~m8[index] + 1;
      mix(index%STATE64M);
      if ( sindex >= STATEM ) {
        sindex = 0;
      } else {
        sindex++;
      }
    }

    mix(0);
    mix(1);
    mix(2);
    mix(3);
    mix(4);
    mix(5);
    mix(6);
  }

  void DISCoHAsH_512_64 ( const void * key, int len, unsigned seed, void * out )
  {
    int tempLen = len;
    if ( tempLen == 0 ) {
      tempLen = 1;
    }
    alignas(uint64_t) uint8_t* tempBuf = new uint8_t[tempLen];
    const uint8_t *key8Arr = (uint8_t *)key;

    alignas(uint64_t) const uint8_t seedbuf[16] = {0};
    const uint8_t *seed8Arr = (uint8_t *)seedbuf;
    const uint64_t *seed64Arr = (uint64_t *)seedbuf;
    uint32_t *seed32Arr = (uint32_t *)seedbuf;

    // the cali number from the Matrix (1999)
    seed32Arr[0] = 0xc5550690;
    seed32Arr[0] -= seed;
    seed32Arr[1] = 1 + seed;
    seed32Arr[2] = ~(1 - seed);
    seed32Arr[3] = (1+seed) * 0xf00dacca;

    // nothing up my sleeve
    dcs[0] = 0x123456789abcdef0;
    dcs[1] = 0x0fedcba987654321;
    dcs[2] = 0xaccadacca80081e5;
    dcs[3] = 0xf00baaf00f00baaa;
    dcs[4] = 0xbeefdeadbeefc0de;
    dcs[5] = 0xabad1deafaced00d;
    dcs[6] = 0xfaceb00cfacec0de;
    dcs[7] = 0xdeadc0dedeadbeef;

    memcpy(tempBuf, key, len);
    uint64_t* temp64 = reinterpret_cast<uint64_t*>(tempBuf);

    round( temp64, key8Arr, len );
    round( seed64Arr, seed8Arr, 16 );
    round( dcs, dcs8, STATE );

    // 512-bit internal state 256-bit output
    uint64_t h[4] = {0}; // This will hold the final 256-bit output

    h[0] -= dcs[2];
    h[0] -= dcs[3];
    h[0] -= dcs[6];
    h[0] -= dcs[7];

    h[1] = dcs[2] ^ dcs[3] ^ dcs[6] ^ dcs[7];

    h[2] -= dcs[0];
    h[2] -= dcs[1];
    h[2] -= dcs[4];
    h[2] -= dcs[5];

    h[3] = dcs[0] ^ dcs[1] ^ dcs[4] ^ dcs[5];

    memcpy(out, h, sizeof(h));  

    delete[] tempBuf;
  }
