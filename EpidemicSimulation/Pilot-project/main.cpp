#include <iostream>
#include <vector>
#include <time.h>
#include "classes.h"
#include "func.h"

Person people[NUM_PEOPLE]; // Array of people
std::default_random_engine generator; // Generator that is used when generating numbers

const int locationArraySize = NUM_HOMES + NUM_WORKPLACES + POPULAR_PLACES;
std::vector<int> locations[locationArraySize];

int main()
{
	int i = 0, dayDuration = 0, simulationTime = 0;
	generator.seed(SEED); // Sets seed of generator
	SetAgents(people, generator);
	SetAgentsHome(people, locations);
	InfectAgents(people);
	
	while (simulationTime <= SIMULATION_DURATION * DAY_DURATION) {

		ChangeAgentsLocation(people, locations, generator, dayDuration);
		while (i < NUM_INTERACTIONS) {
			MakeInteractions(people, locations, generator, locationArraySize - 1);
			++i;
		}
		i = 0;
		dayDuration += 1;
		
		if (dayDuration == DAY_DURATION) {
			simulationTime += dayDuration;
			CheckAgentsHealth(people, locations, generator);
			dayDuration = 0;
			std::cout << "Day: " << simulationTime/DAY_DURATION << " - Infected: " << Person::numInfected << std::endl;
		}
	}

	std::cout << "Infected: " << Person::numInfected << " - " << (float)(100*Person::numInfected)/NUM_PEOPLE << "% of population" << std::endl;
	std::cout << "Recovered: " << Person::numRecovered << " - " << (float)(100 * Person::numRecovered) / NUM_PEOPLE << "% of population" << std::endl;
	std::cout << "Dead: " << Person::numDead << " - " << (float)(100 * Person::numDead) / NUM_PEOPLE << "% of population" << std::endl;
}