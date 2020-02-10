# :city_sunrise: [Discohash](https://github.com/cris691/discohash)

#### **5GB/s** **[SMHasher](https://github.com/cris691/discohash/blob/master/Disco3.result.txt)** 

**Discohash** (*also known as* **BEBB4185**) is a super-fast non-cryptographic hash that passes SMHasher, and runs at 5GB/s in this naive, portable implementation.

------

A super-fast 64-bit non-cryptographic hash.

Tested at ~ 5GB/s @ 3Gz.

Passes all SMHasher tests. 

*Also known as*: **BEBB4185**

## Using

Use the c-code from this repository, or, for a JS implementation:

```console
npm i --save bebb4185
```

Use in Node.JS:

```js
import {disocash} from 'bebb4185'
```

Or using Snowpack as a webmodule:

```js
import {discohash} from './webmodules/bebb4185.js';
```

## JS Implementation

- The JS Implementation produces the same value hashes as the C++ implementation.
- The JS implementation is ~ 1000x slower than the C++ implementation.
- This is probably because of the use of BigInts to stand in for `uint64_t`
- It's possible to implement a 64-bit mult using 32-bit view which would remove the need for BigInt. I have no plan to do this.


## SMHasher verification value

The value is: `BEBB4185`


