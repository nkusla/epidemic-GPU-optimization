#pragma once

#define STATE_VECTOR_LENGTH 624
#define STATE_VECTOR_M 397
#define UPPER_MASK 0x80000000
#define LOWER_MASK 0x7fffffff
#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000

typedef struct {
	int mt[STATE_VECTOR_LENGTH];
	int index;
} MTRand;

MTRand InitSeed(unsigned long seed);

