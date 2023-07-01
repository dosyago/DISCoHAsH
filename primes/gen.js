import * as bcu from 'bigint-crypto-utils';

const L2_10 = Math.log2(10);

//test();
//bigPrimes();
pairPrimes();

async function findAllPrimePairs(prime, millerRabinIterations = 10, desiredRatio = 1.618) {
  const bitLength = prime.toString(2).length;
  
  // Check if the input number is prime
  if (!await bcu.isProbablyPrime(prime, millerRabinIterations)) {
    console.error("The input number is not a prime.");
    return;
  }

  // List of positions with set bits
  let setBitPositions = [];
  for (let i = 0; i < bitLength; i++) {
    if ((prime & (1n << BigInt(i))) !== 0n) {
      setBitPositions.push(i);
    }
  }

  const numberOfCombinations = 1 << setBitPositions.length;

  let foundPair = false;

  // Iterate through combinations
  for (let i = 1; i < numberOfCombinations; i++) {
    let candidate = prime;
    let bitsToUnset = new Set();

    // Randomly unset bits
    for (let j = 0; j < setBitPositions.length; j++) {
      if ((i & (1 << j)) !== 0) {
        const bitPosition = setBitPositions[j];
        candidate &= ~(1n << BigInt(bitPosition));
        bitsToUnset.add(bitPosition);
      }
    }

    // Ensure at least 8 bits are different
    if (bitsToUnset.size < 8) {
      continue;
    }

    // Check if the candidate is a subset of the original number
    if ((candidate | prime) === prime && candidate < prime) {
      // Check if the candidate is prime
      if (await bcu.isProbablyPrime(candidate, millerRabinIterations)) {
        // Check if the numbers differ by the desired ratio
        console.log(`Pair found ${prime} and ${candidate}!\n   ${prime.toString(2)}           ${candidate.toString(2)}`);
        foundPair = true;
      }
    }
  }

  if (!foundPair) {
    console.log("No prime pair found.");
  }
}

function shuffle(array) {
  for (let i = array.length - 1; i > 0; i--) {
    const j = Math.floor(Math.random() * (i + 1));
    [array[i], array[j]] = [array[j], array[i]];
  }
}

async function findAllPrimePairsRan(prime, maxAttempts = 1000, millerRabinIterations = 16, desiredRatio = 1.618) {
  const bitLength = prime.toString(2).length;

  // Check if the input number is prime
  if (!await bcu.isProbablyPrime(prime, millerRabinIterations)) {
    console.error("The input number is not a prime.");
    return;
  }

  // List of positions with set bits
  let setBitPositions = [];
  for (let i = 0; i < bitLength; i++) {
    if ((prime & (1n << BigInt(i))) !== 0n) {
      setBitPositions.push(i);
    }
  }

  let foundPair = false;

  // Random attempts
  for (let attempt = 0; attempt < maxAttempts; attempt++) {
    let candidate = prime;
    shuffle(setBitPositions);

    // Randomly select number of bits to unset
    const bitsToUnsetCount = Math.floor(Math.random() * (setBitPositions.length - 8)) + 8;

    let bitsToUnset = setBitPositions.slice(0, bitsToUnsetCount);

    // Randomly unset bits
    for (const bitPosition of bitsToUnset) {
      candidate &= ~(1n << BigInt(bitPosition));
    }

    // Check if the candidate is a subset of the original number
    if ((candidate | prime) === prime && candidate < prime) {
      // Check if the candidate is prime
      if (await bcu.isProbablyPrime(candidate, millerRabinIterations)) {
        // Check if the numbers differ by the desired ratio
        console.log(`Pair found ${prime} and ${candidate}!\n   ${prime.toString(2)}           ${candidate.toString(2)}`);
        foundPair = true;
      }
    }
  }

  if (!foundPair) {
    console.log("No prime pair found.");
  }
}

async function pairPrimes() {
  for( let i = 0; i < 1000000; i++ ) {
    const b = await newPrime(null, 64);
    await findAllPrimePairs(b);
  }
}
async function bigPrimes() {
  for( let i = 0; i < 10; i++ ) {
    const p = await newPrime(null, 64); 
    const q = await newPrime(null, 63);
    console.log(`${p} (${p.toString(2).length} bits)`);
    console.log(`${q} (${q.toString(2).length} bits)`);
  }
}

async function test() {
  await testPrimes();
  await testSemiPrimes();
}

async function newPrime(digits, bits = null) {
  bits = bits ? bits : Math.ceil(L2_10*digits);

  return bcu.prime(bits, bits>>1);
}

async function testPrimes() {
  for(let i = 3; i < 13; i++ ) {
    console.log({prime:(await newPrime(i)).toString(10)});
  }
}

async function testSemiPrimes() {
  for(let i = 3; i < 13; i++ ) {
    console.log({semiprime:(await newSemiPrime(i)).n.toString(10)});
  }
}

async function newSemiPrime(primeDigits) {
  const p = BigInt(await newPrime(primeDigits));
  const q = BigInt(await newPrime(primeDigits));
  const n = p*q;
  return {n,p,q};
}

async function newTableaux(digits) {
  const {n,p,q} = await newSemiPrime(digits);
  const T = newSquareMatrix(digits);
}

function newSquareMatrix(s) {
  const Rows = new Array(s);
  for( let i = 0; i < s; i++ ) {
    Rows[i] = new Array(s);
  }
  return Rows;
}
