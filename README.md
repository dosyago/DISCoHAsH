# 🚀 [Discohash](https://github.com/dosyago/discohash) - A Blend of Simplicity and Performance in Hashing

[![version](https://img.shields.io/npm/v/bebb4185.svg?label=&color=0080FF)](https://github.com/cris691/discohash/releases/latest) ![npm downloads](https://img.shields.io/npm/dt/bebb4185)

#### **Achieves 8-10GB/s, Successfully Passed [SMHasher Tests](https://github.com/rurban/smhasher/blob/master/doc/BEBB4185.txt)**

---

## Introduction

**Discohash**, also known as **BEBB4185**, is a straightforward and high-performance hash function that combines simplicity with impressive performance. While there are several great high-speed hash functions available, Discohash stands out for its elementary implementation, not relying on architecture-specific optimizations, and still delivering commendable speed and quality. It’s not the fastest nor the best in quality, but its simplicity and portability make it an excellent choice for various applications.

Discohash has passed all the tests in SMHasher and clocks in at 8-10GB/s, depending on hardware. It's one of the fastest hash functions as per [SUPERCOP ECRYPT benchmark](https://bench.cr.yp.to/impl-hash/bebb4185.html).

This repository features a portable, serial implementation in C++. Moreover, an unofficial Go port is available [here](https://github.com/dgryski/go-disco), and there's also a JavaScript version.

---

## Quick Highlights

- 🚀 High-speed 64-bit hash function.
- 🎛 Flexible output sizes ranging from 64 to 256 bits.
- 🛠 Remarkably simple and portable implementation.
- 🧪 Successfully passes all SMHasher tests.
- 🌐 Implemented in C++ with ports to JavaScript and Go.
- 🔧 Includes a simple Command Line Interface (CLI) app for hashing files or standard input from the command line.
- 🔄 Efficient mixing function utilizing multiplications by 64-bit primes, bit rotation, and XOR.

---

## In-depth Cryptanalysis

We extend an open invitation to the cryptographic community for cryptanalysis of Discohash, particularly focusing on the 128-bit variant. This is an excellent opportunity for cryptographers and security enthusiasts to evaluate the strength of Discohash.

If you are able to make a breakthrough or conduct significant analysis, please submit your results as Pull Requests, updating the README with a link to your analysis. Alternatively, you can [contact Cris (me, the author)](mailto:cris@dosyago.com) directly.

*Important Note: To focus on the 128-bit variants, you must modify the source files in C/C++.*

---

## Implementation Details

### Core Mixing Function

The 128-bit to 128-bit core mixing function is succinctly presented below:

```c
  mix(const int A) {
      const int B = A + 1;

      ds[A] *= P;
      ds[A] = rot(ds[A], 23);
      ds[A] *= Q;

      ds[B] ^= ds[A];

      ds[B] *= P;
      ds[B] = rot(ds[B], 23);
      ds[B] *= Q;
  }
```

This function makes use of two 64-bit primes, `P` and `Q`, bit rotation, and XOR operations. The primes `P` and `Q` are defined as:

- P = 0xFFFFFFFFFFFFFFFF - 58 (largest 64-bit prime)
- Q = 13166748625691186689 (random prime)

Interestingly, P and Q have the interesting property that `P - Q = P ^ Q` (their difference equals their XOR).

### Standard Version

Discohash's standard version employs an internal

 state of 256 bits, with the mixing function operating across it. The standard digest size is 64 bits, but you can modify it to produce 128 bits or higher for cryptographic security.

---

## How to Use

#### C++ Implementation:

*The output has the same format as `shasum -a 256 myfile.txt`*

```console
cd src
./build.sh
./bin/discosum myfile.txt
> ... 256-bit hash value as hex ... myfile.txt
```

#### JavaScript Implementation:

Install using npm:

```console
npm i --save bebb4185
```

Use in Node.JS:

```javascript
import {discohash} from 'bebb4185'
```

Usage:

```javascript
const hash = discohash(string_or_typed_array_key, optional_seed);
```

*Note: The JavaScript implementation is ~500x slower than the C++ implementation due to the use of BigInts.*

---

## Golang Port

For Golang enthusiasts, an unofficial port to Go is available [here](https://github.com/dgryski/go-disco).

---

## Chain Zero Prefix Order

This hash when applied iteratively to its own hex output (a chain; with the initial link being a 32-byte input of all zeroes in binary not hex), generates a 2-byte zero prefix after 8,715 iterations, and a 3-byte zero prefix after 8,670,480 iterations:

```
8670467: 630f74d376ad3dc21aef6b2b6522ba385153fc9bad730e045153fc9bad730e04
8670468: 8d3b99e94dd7b62e62b3958909d7802e0e9ddfdee75087a50e9ddfdee75087a5
8670469: 6026eb0fce14e4835f14c0efa9e88363b9ab4053a1ee8b6bb9ab4053a1ee8b6b
8670470: f83dd7560d0b6271e7bd174582ca5d718801aefeeebf46f48801aefeeebf46f4
8670471: aef29abc0eda55952ee2183be8159551a7f4aca19bad85e2a7f4aca19bad85e2
8670472: e6160e8f87accfb9d515ec6f7790ac397ba822756d23b8d07ba822756d23b8d0
8670473: 99803a19074ebd91557fb596f820398d709bbb32784d3896709bbb32784d3896
8670474: 199d695ee4f4ecce0459161c1ac2cc2e9482b241e7fb8ec29482b241e7fb8ec2
8670475: 82b400dd268da3ee6233fcdd160d1bee5defcdb070c223cc5defcdb070c223cc
8670476: 67c5760121db846b07c461fe99d87363d6ec497d53aee544d6ec497d53aee544
8670477: 482ea819bb8b7996b7a91615bb40715092aa897234e15d2e92aa897234e15d2e
8670478: 10b44e30f2eea61fccb431cef28e981f7dca7cc0b91748e57dca7cc0b91748e5
8670479: 0421da4bddc42996e3d821231a33c16073ba1d3140a2b05973ba1d3140a2b059
8670480: 0000009d947ee793ffffff5114791753e4f60afe9a96cf43e4f60afe9a96cf43
Found hash with first three bytes 0x00 after 8670480 iterations.
```

These values are inline with when on average you would expect to land on some particular criterion (~ 1/2 * 2**b) where b is the bit length of the criterion, from a random sequence. So for two bytes (b = 16) you'd expect it around 32768 (with 8,715 being roughly 0.27x) and for 3 bytes (b = 24) you'd expect it around 8388608 (with 8670480 being roughly 1.03x).

---

For detailed information and updates, please visit the [Discohash GitHub repository](https://github.com/dosyago/discohash). Your contributions and feedback are always welcome!
