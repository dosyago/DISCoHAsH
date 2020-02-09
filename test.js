import {discohash} from './index.js';

const tests = [
  [ 'abc123', 0 ],
  [ 'xyz999', 2 ]
];

test();

function test() {
  tests.forEach(([k, s]) => {
    console.log(`${k}, ${s}, 0x${discohash(k, s).toString(16).padStart(16,'0')}`);
  });
}