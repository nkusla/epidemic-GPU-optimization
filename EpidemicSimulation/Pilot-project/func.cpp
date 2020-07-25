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
	int i, random, firstIndex, lastIndex;
	int distancingNum = (int)NUM_PEOPLE * DISTANCING_PERCENTAGE;

	std::vector<int> v;
	SetVector(&v);

	// Set pople who are distancing
	ShuffleVector(&v);
	for (i = 0; i < distancingNum; ++i) {
		people[v[i]].setDistancing(true);
	}

	// Set homeIDs for people
	firstIndex = 0;
	lastIndex = NUM_HOMES - 1;
	std::uniform_int_distribution<int> distributionHome(firstIndex, lastIndex);
	for (i = 0; i < NUM_PEOPLE; ++i) {
		random = distributionHome(generator);
		people[i].setHomeID(random);
	}

	// Set workIDs for people
	firstIndex = lastIndex + 1;
	lastIndex = lastIndex + NUM_WORKPLACES - 1;
	for (i = 0; i < NUM_PEOPLE; ++i) {
		std::uniform_int_distribution<int> distributionWork(firstIndex, lastIndex);
		random = distributionWork(generator);
		people[i].setWorkID(random);
	}

}

void SetAgentsHome(Person* people, std::vector<int>* locations) {
	int i, homeID;

	for (i = 0; i < NUM_PEOPLE; ++i) {
		homeID = people[i].getHomeID();
		locations[homeID].push_back(i);
	}
}

void InfectAgents(Person* people) {
	int initialInfected = (int)NUM_PEOPLE * INITIAL_INFECTIONS;
	
	std::vector<int> v;
	SetVector(&v);
	ShuffleVector(&v);

	for (int i = 0; i < initialInfected; ++i) {
		people[v[i]].setStatus(I);
	}
}

