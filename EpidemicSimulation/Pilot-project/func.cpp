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

void InfectAgents(Person* people) {
	int initialInfected = (int)NUM_PEOPLE * INITIAL_INFECTIONS;
	
	std::vector<int> v;
	SetVector(&v);
	ShuffleVector(&v);

	for (int i = 0; i < initialInfected; ++i) {
		people[v[i]].setStatus(I);
	}
}

void MakeInteractions(Person* people, std::vector<int>* locations, std::default_random_engine generator, int size) {
	int rand, i, j;

	for (i = 0; i <= size; ++i) {

		std::uniform_int_distribution<int> distribution(0, locations[i].size()-1);
		std::vector<int> v = locations[i];
		for (j = 0; j < locations[i].size(); ++j) {
			rand = distribution(generator);
			if (people[v[j]].getStatus() != D && people[v[rand]].getStatus() != D) {
				if (people[v[j]].getStatus() == S && people[v[rand]].getStatus() == S) {
					continue;
				}
				else if (people[v[j]].getStatus() == I && people[v[rand]].getStatus() == S) {
					people[v[rand]].TryInfect(generator, INFECTION_PROBABILITY * 1000);
				}
				else if (people[v[j]].getStatus() == S && people[v[rand]].getStatus() == I) {
					people[v[j]].TryInfect(generator, INFECTION_PROBABILITY * 1000);
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

void ChangeAgentsLocation(Person* people, std::vector<int>* locations, std::default_random_engine generator, int dayDuration) {
	// Upper and lower indexes that will be used when accessing locations of agents
	const int homeIndexFirst = 0;
	const int homeIndexLast = NUM_HOMES - 1;

	const int workIndexFirst = homeIndexLast + 1;
	const int workIndexLast = workIndexFirst + NUM_WORKPLACES - 1;

	const int popularPlaceIndexFirst = workIndexLast + 1;
	const int popularPlaceIndexLast = popularPlaceIndexFirst + POPULAR_PLACES - 1;
	
	std::uniform_int_distribution<int> distributionPopularPlaces(popularPlaceIndexFirst, popularPlaceIndexLast);
	int i, j, locationID, rand;

	// Sends people to work
	if (dayDuration == 0) {
		for (i = homeIndexFirst; i <= homeIndexLast; ++i) {
			std::vector<int> v = locations[i];
			for (j = 0; j < v.size(); ++j) {

			}
		}
	}
	// Sends some people home and some to popular palces
	else if(dayDuration == WORK_HOURS) {
		for (i = workIndexFirst; i <= workIndexLast; ++i) {
			std::vector<int> v = locations[i];
			for (j = 0; j < v.size(); ++j) {
				RemoveAgentFromCurrentLocation(people[v[j]], v[j], locations);
				if (!people[v[j]].getDistancing()) {
					rand = distributionPopularPlaces(generator);
					locations[rand].push_back(v[j]);
					people[v[j]].setCurrentLocation(rand);
				}
				else {
					Person p = people[v[j]];
					locations[p.getHomeID()].push_back(v[j]);
					p.setCurrentLocation(p.getHomeID());
					people[v[j]] = p;
				}
			}
		}
	}
}