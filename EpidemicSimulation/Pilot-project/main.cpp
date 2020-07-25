#include <iostream>
#include <vector>
#include <time.h>
#include "classes.h"
#include "func.h"

Person people[NUM_PEOPLE]; // Array of people
std::default_random_engine generator; // Generator that is used when generating numbers

std::vector<int> locations[NUM_HOMES + NUM_WORKPLACES + POPULAR_PLACES];

// Upper and lower indexes that will be used when accessing locations of agents
const int homeIndexFirst = 0;
const int homeIndexLast = NUM_HOMES - 1;

const int workIndexFirst = homeIndexLast + 1;
const int workIndexLast = workIndexFirst + NUM_WORKPLACES - 1;

const int popularPlaceIndexFirst = workIndexLast + 1;
const int popularPlaceIndexLast = workIndexFirst + POPULAR_PLACES - 1;

int deadPeople, maxInfected, recovered = 0;
int simulationTime = 0;

int main()
{
	int i;
	generator.seed(SEED); // Sets seed of generator
	SetAgents(people, generator);
	SetAgentsHome(people, locations);
	InfectAgents(people);
	

	while (simulationTime <= SIMULATION_DURATION * DAY_DURATION) {

		++simulationTime;
	}

	std::cout << "Hello world!" << std::endl;
}