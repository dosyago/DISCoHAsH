export const BEBB4185 = discohash;

const STATE = 32;
const STATE64 = STATE >> 3;
const STATEM = STATE-1;
const BSTATEM = (STATEM);
const HSTATE64M = (STATE64 >> 1)-1;
const STATE64M = STATE64-1;
const disco_buf = new ArrayBuffer(STATE);
const P = 0xFFFFFFFFFFFFFFFFn - 58n;
const Q = 13166748625691186689n;
const ds8 = new Uint8Array(disco_buf);
const ds32 = new Uint32Array(disco_buf);
const ds = new BigUint64Array(disco_buf);
const C = new BigUint64Array(1);
const C8 = new Uint8Array(1);
const T64 = new BigUint64Array(1);
const T8 = new Uint8Array(1);
const R = 23n;
const S3 = 63n;
const S4 = 64n;

function rot( v, n = 0n ) {
	n = n & S3;
	if (n) {
		v = (v >> n) | (v << (S4-n));
	}
	return v; 
}

function rot8( v, n = 0 ) {
	n = n & 7;
	if (n) {
		v = (v >> n) | (v << (8-n));
	}
	return v; 
}

function mix(A = 0) {
	const B = A+1;
  
	ds[A] *= P;
	ds[A] = rot(ds[A], R);
	ds[A] *= Q;
	
	ds[B] ^= ds[A];

	ds[B] *= P;
	ds[B] = rot(ds[B], R);
	ds[B] *= Q;
}

function round( m64, m8 ) {
	const len = m8.length;
	let index = 0;
	let sindex = 0;
	C[0] = 0xfaccadaccad09997n;
  C8[0] = 137;

	for( let Len = len >> 3; index < Len; index++) {
		T64[0] = m64[index] + BigInt(index) + C[0] + 1n;
    ds[sindex] += rot(T64[0], R);
		C[0] += ~m64[index] + 1n;
		if ( sindex == HSTATE64M ) {
			mix(0);
		} else if ( sindex == STATE64M ) {
			mix(2);
			sindex = -1;
		}
		sindex++;
	}

	mix(1);

	index <<= 3;
	sindex = index&(BSTATEM);
	for( ; index < len; index++) {
		T8[0] = m8[index] + index + C8[0] + 1;
    ds8[sindex] = rot8(T8[0], 23);
		C8[0] += ~m8[sindex] + 1;
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

export function discohash( key = '', seed = 0 ) {
  // get 32-bit codepoints if it's a string
    // ref: "I say we take take-off, nuke the site from orbit, only way to be sure"
  if ( typeof key == "string" ) {
    const Key = [];
    for( const k of key ) {
      Key.push(k.codePointAt(0));
    }
    key = Key;
  }

  let len = key.length*4;
  if( len % 8 != 0 ) {
    len += 4;
  }

  const key_buf = new ArrayBuffer(len);
  const key8Arr = new Uint8Array(key_buf);
  const key32Arr = new Uint32Array(key_buf);
  const key64Arr = new BigUint64Array(key_buf);

  key32Arr.set(key);

  const seed_buf = new ArrayBuffer(16);
  const seed8Arr = new Uint8Array(seed_buf);
  const seed32Arr = new Uint32Array(seed_buf);
  const seed64Arr = new BigUint64Array(seed_buf);

  seed32Arr[0] = 0xc5550690;
  seed32Arr[0] -= seed;
  // if seed mod doesn't work let's try reverse order of seed/key round calls
  seed32Arr[1] = 1 + seed;
  seed32Arr[2] = ~(1 - seed);
  seed32Arr[3] = (1+seed) * 0xf00dacca;

  // nothing up my sleeve
  ds[0] = 0x123456789abcdef0n;
  ds[1] = 0x0fedcba987654321n;
  ds[2] = 0xaccadacca80081e5n;
  ds[3] = 0xf00baaf00f00baaan;

  round( key64Arr, key8Arr );
  round( seed64Arr, seed8Arr );
  round( ds, ds8 );

  /**
  console.log(`ds = ${ds[0]} ${ds[1]} ${ds[2]} ${ds[3]}\n`);
  **/

  const output = new ArrayBuffer(STATE);
  const h = new BigUint64Array(output);

  h[0] = ds[2];
  h[1] = ds[3];

  h[0] += h[1];

  return h[0];
}
