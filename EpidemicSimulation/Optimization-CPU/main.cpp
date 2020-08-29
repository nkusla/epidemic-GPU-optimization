#include <iostream>
#include <vector>
#include <chrono>
#include "person.h"
#include "func.h"

extern std::vector<int> generatedRandNumbers;

Person people[NUM_PEOPLE]; // Array of people
std::default_random_engine generator; // Generator that is used when generating numbers
std::vector<int> rngVector; // Stores all random number that have been generated

const int locationArraySize = NUM_HOMES + NUM_WORKPLACES + POPULAR_PLACES;
std::vector<int> locations[locationArraySize];

std::string outputHistory; // Stores output to console
std::string date = GetCurrentDate(); // Date is used for file names

int main()
{
	int i = 0, dayDuration = 0, simulationTime = 0;
	generator.seed(SEED); // Sets seed of generator
	InitiateAgents(people, generator);
	SetAgentsHome(people, locations);
	InfectAgents(people, generator);

	auto start = std::chrono::high_resolution_clock::now();

	while (simulationTime < SIMULATION_DURATION * DAY_DURATION) {

		ChangeAgentsLocation(people, locations, generator, dayDuration, rngVector);
		while (i < NUM_INTERACTIONS) {
			MakeInteractions(people, locations, generator, locationArraySize - 1, rngVector);
			++i;
		}
		i = 0;
		dayDuration += 1;
		
		if (dayDuration == DAY_DURATION) {
			simulationTime += dayDuration;
			dayDuration = 0;

			WriteInfo(simulationTime, outputHistory);
			Person::changeMaxNewInfected();

			CheckAgentsStatus(people, locations, generator, rngVector);
			//LogGeneratedRandomNumbers(rngVector, date);
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	int executionTime = duration.count();

	SimulationEndInfo(outputHistory, executionTime);
	LogSimulationParameters(outputHistory, date);
}