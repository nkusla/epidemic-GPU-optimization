#include <iostream>
#include <vector>
#include "func.h"

void ShuffleVector(std::vector<int>* p) {
	std::shuffle(p->begin(), p->end(), std::default_random_engine(SEED));
}

void SetVector(std::vector<int>* p) {
	for (int i = 0; i < NUM_PEOPLE; ++i) {
		p->push_back(i);
	}
}

void SetAgents(Person* people, std::default_random_engine generator) {
	int i, random = 0;
	int distancingNum = (int)NUM_PEOPLE * DISTANCING_PERCENTAGE;

	std::vector<int> v;
	SetVector(&v);

	// Set pople who are distancing
	ShuffleVector(&v);
	for (i = 0; i < distancingNum; ++i) {
		people[v[i]].setDistancing(true);
	}

	// Set homeIDs for people
	std::uniform_int_distribution<int> distributionHome(0, NUM_HOMES-1);
	for (i = 0; i < NUM_PEOPLE; ++i) {
		random = distributionHome(generator);
		people[i].setHomeID(random);
	}

	// Set workIDs for people
	for (i = 0; i < NUM_PEOPLE; ++i) {
		std::uniform_int_distribution<int> distributionWork(0, NUM_WORKPLACES-s1);
		random = distributionWork(generator);
		people[i].setWorkID(random);
	}


}