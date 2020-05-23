#ifndef _DISCOHASH_H_
#define _DISCOHASH_H_
#include <cstdint>

#if defined(_MSC_VER) && (_MSC_VER < 1600)
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned __int64 uint64_t;
#endif // !defined(_MSC_VER)

void BEBB4185_64          ( const void * key, int len, uint32_t seed, void * out );

#endif // _DISCOHASH_H_
