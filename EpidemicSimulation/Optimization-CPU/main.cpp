#include <iostream>
#include <vector>
#include <chrono>
#include "person.h"
#include "func.h"

std::vector<int> generatedRandNumbers;

Person people[NUM_PEOPLE]; // Array of people
std::default_random_engine generator(SEED); // Generator that is used when generating numbers

const int locationArraySize = NUM_HOMES + NUM_WORKPLACES + POPULAR_PLACES;
std::vector<int> locations[locationArraySize];

std::string outputHistory; // Stores output to console
std::string deviceType = "CPU";
std::string date = GetCurrentDate(); // Date is used for file names

int main()
{
	int i = 0, dayDuration = 0, simulationTime = 0, maxLocationSize = 0;
	InitiateAgents(people, generator);
	SetAgentsHome(people, locations);
	InfectAgents(people, generator);

	std::cout << "DEVICE TYPE: " << deviceType << "\n" << std::endl;;
	std::cout << "Simulation start: \n" << std::endl;

	auto start = std::chrono::high_resolution_clock::now();

	while (simulationTime < SIMULATION_DURATION * DAY_DURATION) {

		//GetMaxPeopleOnLocation(locations, locationArraySize, maxLocationSize);
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

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	int executionTime = duration.count();

	SimulationEndInfo(outputHistory, executionTime, maxLocationSize);

	LogSimulationParameters(outputHistory, date, resultsPathCPU, deviceType);
}