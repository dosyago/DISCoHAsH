#ifndef _DISCOHASH_H_
#define _DISCOHASH_H_

// gotten from random.org
// as hex bytes that I formatted into 64-bit values

#include <vector>
#include <array>
#include <stdint.h>

class DiscoHash {
  private:
    static const int STATE = 32;
    static const int STATE64 = STATE / 8;
    static const int STATEM = STATE-1;
    static const int HSTATE64M = (STATE64 >> 1)-1;
    static const int STATE64M = STATE64-1;
    static const uint64_t P = 0xFFFFFFFFFFFFFFFF - 58;
    static const uint64_t Q = 13166748625691186689U;
    
    std::vector<uint64_t> ds;
    std::vector<uint8_t> ds8;

    uint64_t rot(uint64_t v, int n) {
      n = n & 63U;
      if (n)
        v = (v >> n) | (v << (64-n));
      return v; 
    }

    uint8_t rot8(uint8_t v, int n) {
      n = n & 7U;
      if (n)
        v = (v >> n) | (v << (8-n));
      return v; 
    }

    void mix(const int A) {
      const int B = A+1;
      ds[A] *= P;
      ds[A] = rot(ds[A], 23);
      ds[A] *= Q;

      ds[B] ^= ds[A];

      ds[B] *= P;
      ds[B] = rot(ds[B], 23);
      ds[B] *= Q;
    }

    void round(const uint64_t* m64, const uint8_t* m8, int len) {
      int index = 0;
      int sindex = 0;
      int Len = len >> 3;
      uint64_t counter = 0xfaccadaccad09997;
      uint8_t counter8 = 137;


      for(index = 0; index < Len; index++) {
        std::cout << "[IN LOOP 1] Len: " << Len << ", len: " << len << ", index: " << index << ", sindex: " << sindex << std::endl;

        ds[sindex] += rot(m64[index] + index + counter + 1, 23);
        counter += ~m64[index] + 1;
        if ( sindex == HSTATE64M ) {
          mix(0);
        } else if ( sindex == STATE64M ) {
          mix(2);
          sindex = -1;
        }
        sindex++;
      }

      mix(1);


      sindex = index&(STATEM);
      index = index << 3;
      
      for(; index < len; index++) {
        std::cout << "[IN LOOP 2] Len: " << Len << ", len: " << len << ", index: " << index << ", sindex: " << sindex << std::endl;
        ds8[sindex] += rot8(m8[index] + index + counter8 + 1, 23);
        counter8 += ~m8[sindex] + 1;
        mix(index%STATE64M);
        if ( sindex >= STATEM ) {
          sindex = -1;
        }
        sindex++;
      }

      mix(0);
      mix(1);
      mix(2);

    }

  public:
    DiscoHash() : ds(STATE64, 0), ds8(STATE, 0) {

    }

    void BEBB4185_64 ( const void * key, int len, uint32_t seed, void * out ) {
      const uint8_t *key8Arr = reinterpret_cast<const uint8_t*>(key);
      const uint64_t *key64Arr = reinterpret_cast<const uint64_t*>(key);

      uint32_t seedbuf[4] = {0};
      const uint8_t *seed8Arr = reinterpret_cast<uint8_t*>(seedbuf);
      const uint64_t *seed64Arr = reinterpret_cast<uint64_t*>(seedbuf);

      seedbuf[0] = 0xc5550690;
      seedbuf[0] -= seed;
      seedbuf[1] = 1 + seed;
      seedbuf[2] = ~(1 - seed);
      seedbuf[3] = (1+seed) * 0xf00dacca;

      ds[0] = 0x123456789abcdef0;
      ds[1] = 0x0fedcba987654321;
      ds[2] = 0xaccadacca80081e5;
      ds[3] = 0xf00baaf00f00baaa;

      std::vector<uint64_t> key64(len / sizeof(uint64_t));
      std::vector<uint8_t> key8(len);
      std::copy(key64Arr, key64Arr + len / sizeof(uint64_t), key64.begin());
      std::copy(key8Arr, key8Arr + len, key8.begin());
 
      round( key64.data(), key8.data(), len );

      std::array<uint64_t, STATE64> seed64;
      std::array<uint8_t, STATE> seed8;
      std::copy(seed64Arr, seed64Arr + 4, seed64.begin());
      std::copy(seed8Arr, seed8Arr + 16, seed8.begin());

      round( seed64.data(), seed8.data(), 16 );

      round( ds.data(), ds8.data(), STATE );
      
      std::array<uint64_t, 4> h = {ds[2] + ds[3], ds[3], ds[0] + ds[1], ds[1]};

      std::copy(h.begin(), h.end(), reinterpret_cast<uint64_t*>(out));
      std::cout << "Copy out done" << h[0] << " " << h[1] << " " << h[2] << " " << h[3] << std::endl;
    }
};

#endif // _DISCOHASH_H_

