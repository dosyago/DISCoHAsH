# :city_sunrise: [Discohash](https://github.com/cris691/discohash)

#### **[5GB/s SMHasher](https://github.com/cris691/discohash/blob/master/Disco3.result.txt)**  [![version](https://img.shields.io/npm/v/bebb4185.svg?label=&color=0080FF)](https://github.com/cris691/discohash/releases/latest) ![npm downloads](https://img.shields.io/npm/dt/bebb4185)

**Discohash** (*also known as* **BEBB4185**) is a super-fast non-cryptographic hash that passes all of SMHasher, and runs at 5GB/s in this naive, portable implementation.

------

# Quick Facts

- A super-fast 64-bit non-cryptographic hash.
- Tested at ~ 5GB/s @ 3Gz.
- Passes all SMHasher tests. 
- *Also known as*: **BEBB4185**
- Implemented in C++, and also a port to JS
- This repo includes a simple CLI app for hashing files or stdin from the command line.

## Using

Use the C code from this repository, either in your project or as a CL-app (included):

```console
cd src
./build.sh
./bin/bebbsum < test.txt
> 0xa2a647993898a3df
./bin/bebbsum test.txt
> 0xa2a647993898a3df
```

or, for a JS implementation:

```console
npm i --save bebb4185
```

Use in Node.JS:

```js
import {discohash} from 'bebb4185'
```

Or using Snowpack as a webmodule:

```js
import {discohash} from './web_modules/bebb4185.js';
```

Then call it:

```js
const hash = discohash(string_or_typed_array_key, optional_seed);
```

## JS Implementation

- The JS Implementation produces the same value hashes as the C++ implementation.
- The JS implementation is ~ 1000x slower than the C++ implementation.
- This is probably because of the use of BigInts to stand in for `uint64_t`
- It's possible to implement a 64-bit mult using 32-bit view which would remove the need for BigInt. I have no plan to do this.

## SMHasher verification value

The value is: `BEBB4185`


