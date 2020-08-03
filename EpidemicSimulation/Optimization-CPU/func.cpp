#include "func.h"

void RandomNoRepetition(std::vector<int>& v, std::default_random_engine& generator) {
	for (int i = 0; i < NUM_PEOPLE; ++i) {
		v.push_back(i);
	}

	std::shuffle(v.begin(), v.end(), generator);
}

void SetAgents(Person* people, std::default_random_engine& generator) {
	int i, random, firstIndex, lastIndex;
	int distancingNum = static_cast<int>(NUM_PEOPLE * DISTANCING_PERCENTAGE);

	std::vector<int> v;

	// Set people who are distancing
	RandomNoRepetition(v, generator);
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
	std::uniform_int_distribution<int> distributionWork(firstIndex, lastIndex);
	for (i = 0; i < NUM_PEOPLE; ++i) {
		random = distributionWork(generator);
		people[i].setWorkID(random);
	}

}

void SetAgentsHome(Person* people, std::vector<int>* locations) {
	int i, homeID;

	for (i = 0; i < NUM_PEOPLE; ++i) {
		homeID = people[i].getHomeID();
		locations[homeID].push_back(i);
		people[i].setCurrentLocation(homeID);
	}
}

void InfectAgents(Person* people, std::default_random_engine& generator) {
	int initialInfected = static_cast<int>(NUM_PEOPLE * INITIAL_INFECTIONS);
	
	std::vector<int> v;

	RandomNoRepetition(v, generator);
	for (int i = 0; i < initialInfected; ++i) {
		people[v[i]].setStatus(I);
		Person::numInfected++;
		Person::maxInfected++;
	}
}

void MakeInteractions(Person* people, std::vector<int>* locations, std::default_random_engine& generator, int size) {
	int rand, i, j;
	std::vector<int> v;

	for (i = 0; i <= size; ++i) {

		if (locations[i].size() > 1) {
			std::uniform_int_distribution<int> distribution(0, locations[i].size()-1);
			v = locations[i];
			for (j = 0; j < locations[i].size(); ++j) {
				rand = distribution(generator);
				if (people[v[j]].getStatus() == I && people[v[rand]].getStatus() == S) {
					people[v[rand]].TryInfect(generator, INFECTION_PROBABILITY * 100000);
				}
				else if (people[v[j]].getStatus() == S && people[v[rand]].getStatus() == I) {
					people[v[j]].TryInfect(generator, INFECTION_PROBABILITY * 100000);
				}
				else { continue; }
			}
		}
	}

}

void RemoveAgentFromCurrentLocation(Person person, int personIndex, std::vector<int>* locations) {
	int currentLocation = person.getCurrentLocation();
	std::vector<int> v = locations[currentLocation];

	for (auto it = v.begin(); it != v.end(); ++it) {
		if (personIndex == *it) {
			v.erase(it);
			break;
		}
	}

	locations[currentLocation] = v;
}

void ChangeAgentsLocation(Person* people, std::vector<int>* locations, std::default_random_engine& generator, int dayDuration) {
	// Upper and lower indexes that will be used when accessing locations of agents
	const int homeIndexFirst = 0;
	const int homeIndexLast = NUM_HOMES - 1;

	const int workIndexFirst = homeIndexLast + 1;
	const int workIndexLast = workIndexFirst + NUM_WORKPLACES - 1;

	const int popularPlaceIndexFirst = workIndexLast + 1;
	const int popularPlaceIndexLast = popularPlaceIndexFirst + POPULAR_PLACES - 1;
	
	std::uniform_int_distribution<int> distributionPopularPlaces(popularPlaceIndexFirst, popularPlaceIndexLast);
	std::vector<int> v;
	int i, j, locationID;

	// Sends people to work
	if (dayDuration == 0) {
		for (i = homeIndexFirst; i <= homeIndexLast; ++i) {
			v = locations[i];
			for (j = 0; j < v.size(); ++j) {
				RemoveAgentFromCurrentLocation(people[v[j]], v[j], locations);
				locationID = people[v[j]].getWorkID();
				locations[locationID].push_back(v[j]);
				people[v[j]].setCurrentLocation(locationID);
			}
		}
	}
	// Sends some people home and some to popular places
	else if(dayDuration == WORK_HOURS) {
		for (i = workIndexFirst; i <= workIndexLast; ++i) {
			v = locations[i];
			for (j = 0; j < v.size(); ++j) {
				RemoveAgentFromCurrentLocation(people[v[j]], v[j], locations);
				if (!people[v[j]].getDistancing()) {
					locationID = distributionPopularPlaces(generator);
					locations[locationID].push_back(v[j]);
					people[v[j]].setCurrentLocation(locationID);
				}
				else {
					locationID = people[v[j]].getHomeID();
					locations[locationID].push_back(v[j]);
					people[v[j]].setCurrentLocation(locationID);
				}
			}
		}
	}
	// Sends people from one popular location to the other
	else if (dayDuration == WORK_HOURS + LOCATION_DURATION) {
		for (i = popularPlaceIndexFirst; i <= popularPlaceIndexLast; ++i) {
			v = locations[i];
			for (j = 0; j < v.size(); ++j) {
				RemoveAgentFromCurrentLocation(people[v[j]], v[j], locations);
				locationID = distributionPopularPlaces(generator);
				locations[locationID].push_back(v[j]);
				people[v[j]].setCurrentLocation(locationID);
			}
		}
	}
	else if (dayDuration == WORK_HOURS + 2 * LOCATION_DURATION) {
		for (i = popularPlaceIndexFirst; i <= popularPlaceIndexLast; ++i) {
			v = locations[i];
			for (j = 0; j < v.size(); ++j) {
				RemoveAgentFromCurrentLocation(people[v[j]], v[j], locations);
				locationID = people[v[j]].getHomeID();
				locations[locationID].push_back(v[j]);
				people[v[j]].setCurrentLocation(locationID);
			}
		}
	}
	else { 
	}
}

void CheckAgentsHealth(Person* people, std::vector<int>* locations, std::default_random_engine& generator) {	
	for (int i = 0; i < NUM_PEOPLE; ++i) {
		if (people[i].getStatus() == I) {
			if (people[i].TryKill(generator, FATALITY_RATE * 100000)) {
				RemoveAgentFromCurrentLocation(people[i], i, locations);
			}
			else {
				people[i].ExtendInfectionDay();
				if (people[i].getInfectionDays() == INFECTION_DURATION) {
					people[i].RecoverAgent();
				}
			}
		}
	}
}

std::string GetCurrentDate() {
	time_t now = time(0);
	tm* tstruct;
	char buffer[40];
	tstruct = localtime(&now);
	strftime(buffer, sizeof(buffer), "%Y.%m.%d. %H-%M-%S", tstruct);

	return buffer;
}

void WriteInfo(int simulationTime, std::string& outputHistory) {
	std::string output;
	output = "Day " + std::to_string(simulationTime / DAY_DURATION) + " - Infected: " +
		std::to_string(Person::numInfected) + " - new cases: " + std::to_string(Person::newInfected) + "\n";

	std::cout << output;

	outputHistory += "\t" + output;
}

void SimulationEndInfo(std::string& outputHistory) {

	std::string output;
	double value;

	value = static_cast<double>(100 * Person::maxInfected) / NUM_PEOPLE;
	output += "\nMax infected: " + std::to_string(Person::maxInfected) + " - " + std::to_string(value) + "% of population\n";

	value = static_cast<double>(100 * Person::maxNewInfected) / NUM_PEOPLE;
	output += "Peak of epidemic: " + std::to_string(Person::maxNewInfected) + " - " + std::to_string(value) + "\n";

	value = static_cast<double>(100 * Person::numInfected) / NUM_PEOPLE;
	output += "\nInfected: " + std::to_string(Person::numInfected) + " - " + std::to_string(value) + "% of population\n";

	value = static_cast<double>(100 * Person::numRecovered) / NUM_PEOPLE;
	output += "Recovered: " + std::to_string(Person::numRecovered) + " - " + std::to_string(value) + "% of population\n";

	value = static_cast<double>(100 * Person::numDead) / NUM_PEOPLE;
	output += "Dead: " + std::to_string(Person::numDead) + " - " + std::to_string(value) + "% of population\n";

	std::cout << output;

	outputHistory += output;
}

void LogSimulationParameters(std::string& outputHistory) {
	std::ofstream file;
	std::string date = GetCurrentDate();
	std::string fileName = "../../Results/" + date + ".txt";

	file.open(fileName);

	std::string params;
	params += "Parameters:\n\n";
	params += "SEED " + std::to_string(SEED) + "\n\n";

	params += "NUM_PEOPLE " + std::to_string(NUM_PEOPLE) + "\n";
	params += "POPULAR_PLACES " + std::to_string(POPULAR_PLACES) + "\n";
	params += "NUM_HOMES " + std::to_string(NUM_HOMES) + "\n";
	params += "NUM_WORKPLACES " + std::to_string(NUM_WORKPLACES) + "\n";
	params += "NUM_INTERACTIONS " + std::to_string(NUM_INTERACTIONS) + "\n\n";

	params += "INITIAL_INFECTIONS " + std::to_string(INITIAL_INFECTIONS) + "\n";
	params += "DISTANCING_PERCENTAGE " + std::to_string(DISTANCING_PERCENTAGE) + "\n";
	params += "INFECTION_PROBABILITY " + std::to_string(INFECTION_PROBABILITY) + "\n";
	params += "FATALITY_RATE " + std::to_string(FATALITY_RATE) + "\n\n";

	params += "INFECTION_DURATION " + std::to_string(INFECTION_DURATION) + "\n";
	params += "SIMULATION_DURATION " + std::to_string(SIMULATION_DURATION) + "\n\n";

	params += "WORK_HOURS " + std::to_string(WORK_HOURS) + "\n";
	params += "LOCATION_DURATION " + std::to_string(LOCATION_DURATION) + "\n";
	params += "DAY_DURATION " + std::to_string(DAY_DURATION) + "\n\n";

	params += "--------------------------------------------------------\n\n";

	file << params;
	file << "Simulation history: \n\n";
	file << outputHistory;
	file.close();
}