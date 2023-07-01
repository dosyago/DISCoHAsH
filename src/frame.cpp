// Copyright 2023 Cris Stringfellow & Dosyago
#include <cstdio>
#include <inttypes.h>
#include <cstring>
#include <iostream>
#include "discohash.h"

constexpr int STATE = 64;
alignas(uint64_t) uint8_t disco_buf[STATE] = {0};
alignas(uint64_t) uint8_t *ds8 = (uint8_t *)disco_buf;
uint64_t *ds = (uint64_t *)disco_buf;

  static inline void mix(const int i)
  {
    // implement key mixing function
  }

  static inline void round( const uint64_t * m64, const uint8_t * m8, int len )
  {
    int index = 0;
    int sindex = 0;

    for( int Len = len >> 3; index < Len; index++) {
      ds[sindex] += m64[index] + index + 1;

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
    sindex = index&(STATE-1);

    for( ; index < len; index++) {
      ds8[sindex] += m8[index] + index + 1;

      mix(index%((STATE>>3)- 1));

      if ( sindex >= (STATE-1) ) {
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

  void hash_func_64 ( const void * key, int len, unsigned seed, void * out )
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
    seed32Arr[0] = 0;
    seed32Arr[0] -= seed;
    seed32Arr[1] = 1 + seed;
    seed32Arr[2] = ~(1 - seed);
    seed32Arr[3] = (1+seed) * seed32Arr[2];

    // nothing up my sleeve
    ds[0] = 0x1111111111111111;
    ds[1] = 0x5555555555555555;
    ds[2] = 0xAAAAAAAAAAAAAAAA;
    ds[3] = 0xEEEEEEEEEEEEEEEE;

    memcpy(tempBuf, key, len);
    uint64_t* temp64 = reinterpret_cast<uint64_t*>(tempBuf);

    round( temp64, key8Arr, len );
    round( seed64Arr, seed8Arr, 16 );
    round( ds, ds8, STATE );

    // 512-bit internal state 64-bit output
    uint64_t h[1] = {0}; 

    h[0] -= ds[2];
    h[0] -= ds[3];
    h[0] -= ds[6];
    h[0] -= ds[7];

    memcpy(out, h, sizeof(h)); 

    delete[] tempBuf;
  }
