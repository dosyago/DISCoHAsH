#pragma once

#ifndef DISCOHASH_H
#define DISCOHASH_H

#include <stdint.h>
#include <string.h>

static const int STATE = 32;
static const int STATE64 = STATE / 8;
static const int STATEM = STATE-1;
static const int HSTATE64M = (STATE64 >> 1)-1;
static const int STATE64M = STATE64-1;
static const uint64_t P = 0xFFFFFFFFFFFFFFFF - 58;
static const uint64_t Q = 13166748625691186689U;

inline static uint64_t rot(uint64_t v, int n) {
    n = n & 63U;
    if (n)
        v = (v >> n) | (v << (64-n));
    return v;
}

inline static uint8_t rot8(uint8_t v, int n) {
    n = n & 7U;
    if (n)
        v = (v >> n) | (v << (8-n));
    return v;
}

inline static void mix(uint64_t* ds, const int A) {
    const int B = A+1;
    ds[A] *= P;
    ds[A] = rot(ds[A], 23);
    ds[A] *= Q;

    ds[B] ^= ds[A];

    ds[B] *= P;
    ds[B] = rot(ds[B], 23);
    ds[B] *= Q;
}

inline void round(const uint64_t* m64, const uint8_t* m8, int len, uint64_t* ds, uint8_t* ds8) {
    int index = 0;
    int sindex = 0;
    int Len = len >> 3;
    uint64_t counter = 0xfaccadaccad09997;
    uint8_t counter8 = 137;

    for(index = 0; index < Len; index++) {
        ds[sindex] += rot(m64[index] + index + counter + 1, 23);
        counter += ~m64[index] + 1;
        if (sindex == HSTATE64M) {
            mix(ds, 0);
            sindex++;
        } else if (sindex == STATE64M) {
            mix(ds, 2);
            sindex = 0;
        } else {
          sindex++;
        }
    }

    mix(ds, 1);

    sindex = index & (STATEM);
    index = index << 3;

    for(; index < len; index++) {
        ds8[sindex] += rot8(m8[index] + index + counter8 + 1, 23);
        counter8 += ~m8[sindex] + 1;
        mix(ds, index % STATE64M);
        if (sindex >= STATEM) {
          sindex = 0;
        } else {
          sindex++;
        }
    }

    mix(ds, 0);
    mix(ds, 1);
    mix(ds, 2);
}

inline void BEBB4185_64(const void* key, int len, uint32_t seed, void* out) {
    const uint8_t *key8Arr = reinterpret_cast<const uint8_t*>(key);

    uint32_t seedbuf[4] = {0};

    seedbuf[0] = 0xc5550690;
    seedbuf[0] -= seed;
    seedbuf[1] = 1 + seed;
    seedbuf[2] = ~(1 - seed);
    seedbuf[3] = (1 + seed) * 0xf00dacca;

    uint64_t ds[STATE64] = {
        0x123456789abcdef0,
        0x0fedcba987654321,
        0xaccadacca80081e5,
        0xf00baaf00f00baaa
    };

    uint8_t ds8[STATE] = {0};

    std::vector<uint8_t> key8(key8Arr, key8Arr + len);
    std::vector<uint64_t> key64(len / sizeof(uint64_t));
    memcpy(key64.data(), key8Arr, (len / sizeof(uint64_t)) * sizeof(uint64_t));

    round(key64.data(), key8.data(), len, ds, ds8);

    uint64_t seed64[4];
    uint8_t seed8[16];
    memcpy(seed64, seedbuf, sizeof(seedbuf));
    memcpy(seed8, seedbuf, sizeof(seedbuf));

    round(seed64, seed8, 16, ds, ds8);

    round(ds, ds8, STATE, ds, ds8);

    uint64_t h[4] = {ds[2] + ds[3], ds[3], ds[0] + ds[1], ds[1]};

    memcpy(out, h, sizeof(h));
}

#endif // DISCOHASH_H

