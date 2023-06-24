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

If you are able to make a breakthrough or conduct significant analysis, please submit your results as Pull Requests, updating the README with a link to your analysis. Alternatively, [contact Cris](mailto:cris@dosycorp.com) directly for communication.

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

For detailed information and updates, please visit the [Discohash GitHub repository](https://github.com/cris691/discohash). Your contributions and feedback are always welcome!
