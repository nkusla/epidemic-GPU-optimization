#include <iostream>
#include <vector>
#include "person.h"
#include "func.h"

Person people[NUM_PEOPLE]; // Array of people
std::default_random_engine generator; // Generator that is used when generating numbers

const int locationArraySize = NUM_HOMES + NUM_WORKPLACES + POPULAR_PLACES;
std::vector<int> locations[locationArraySize];
std::string outputHistory;

int main()
{
	int i = 0, dayDuration = 0, simulationTime = 0;
	generator.seed(SEED); // Sets seed of generator
	InitiateAgents(people, generator);
	SetAgentsHome(people, locations);
	InfectAgents(people, generator);

	while (simulationTime < SIMULATION_DURATION * DAY_DURATION) {

		ChangeAgentsLocation(people, locations, generator, dayDuration);
		while (i < NUM_INTERACTIONS) {
			MakeInteractions(people, locations, generator, locationArraySize - 1);
			++i;
		}
		i = 0;
		dayDuration += 1;
		
		if (dayDuration == DAY_DURATION) {
			simulationTime += dayDuration;
			dayDuration = 0;

			WriteInfo(simulationTime, outputHistory);
			Person::changeMaxNewInfected();

			CheckAgentsStatus(people, locations, generator);
		}
	}

	SimulationEndInfo(outputHistory);
	LogSimulationParameters(outputHistory);
}