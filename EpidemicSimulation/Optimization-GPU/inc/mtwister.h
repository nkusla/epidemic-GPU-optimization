// Struct of Mersenne Twister engine

#pragma once

#define STATE_VECTOR_LENGTH 624

typedef struct {
	int mt[STATE_VECTOR_LENGTH];
	int index;
} MTRand;

