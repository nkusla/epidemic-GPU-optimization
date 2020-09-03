#include "mtwister.h"

MTRand InitSeed(unsigned long seed) {
	MTRand rand;
	rand.mt[0] = seed & 0xffffffff;
	for (rand.index = 1; rand.index < STATE_VECTOR_LENGTH; rand.index++) {
		rand.mt[rand.index] = (6069 * rand.mt[rand.index - 1]) & 0xffffffff;
	}

	return rand;
}
