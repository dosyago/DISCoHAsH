import {discohash} from './index.js';

const tests = [
  [ 'abc123', 0 ],
  [ 'xyz999', 2 ]
];

test();
smhasher_verification_value();
big_key_value();
big_key_value();
big_key_value();
big_key_value();
big_key_value();

function test() {
  tests.forEach(([k, s]) => {
    console.log(`${k}, ${s}, 0x${discohash(k, s).toString(16).padStart(16,'0')}`);
  });
}

function swap(a,i,b,j) {
  a[i] ^= b[j];
  b[j] ^= a[i];
  a[i] ^= b[j];
}

function swapb(a,i) {
  a[i] = (a[i] << 4) | (a[i] >> 4);
}

function smhasher_verification_value() {
  // Copied from <smhasher_repo>/src/KeysetTests.cpp
  
  const hashbytes = 8;
  const key = new Uint8Array( 256 );
  const hbuf = new ArrayBuffer(256*hashbytes);
  const hashes = new Uint8Array(hbuf);
  const out = new ArrayBuffer(hashbytes);
  const hash = new Uint8Array(out);
  const h32 = new Uint32Array(out);
  const hout = new BigUint64Array(out);
  const verif = new Uint32Array(1);

  // Hash keys of the form {0}, {0,1}, {0,1,2}... up to N=255,using 256-N as
  // the seed
  for ( let i = 0; i < 256; i++ ) {
    key[i] = i;

    const seed = 256-i;

    hout[0] = discohash(key.slice(0,i), seed );

    hashes.set( hash, i*hashbytes );
  }

  // Then hash the result array

  hout[0] = discohash(hashes, 0);
  // equivalentj
  // verif[0] = hash[0] | (hash[1] << 8) | (hash[2] << 16) | (hash[3] << 24);
  /**
  swap(hash,0,hash,2);
  swap(hash,1,hash,3);
  swap(hash,2,hash,3);
  swapb(hash,0);
  **/
  verif[0] = h32[0];

  console.log(`JS verif val: 0x${verif[0].toString(16).padStart(8, '0')}`);
  console.log(`JS verif val: 0x${verif[0].toString(2).padStart(32, '0')}`);
  
  return verif[0];
}

function big_key_value() {
  const A = new Array(51000).join('.').split('.');
  const B = A.map((_,i) => {
    return String.fromCodePoint(i);
  });
  const C = B.join('');

  const start = Date.now();
  const hash = discohash(C, 9999);
  console.log("Hash", hash);
  const end = Date.now();

  const duration = (end-start);

  const rate = (C.length*4*1000.0/duration)/1000000;
  console.log("JS Rate MB/s", rate.toFixed(2));
}

