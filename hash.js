export const BEBB4185 = discohash;

const STATE = 32;
const STATE64 = STATE >> 3;
const STATEM = STATE-1;
const HSTATE64M = (STATE64 >> 1)-1;
const STATE64M = STATE64-1;
const disco_buf = new ArrayBuffer(STATE);
const P = 0xFFFFFFFFFFFFFFFFn - 58n;
const Q = 13166748625691186689n;
const ds8 = new Uint8Array(disco_buf);
const ds32 = new Uint32Array(disco_buf);
const ds = new BigUint64Array(disco_buf);

function round( m64, m8 ) {

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

  const len = key.length*4;
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
