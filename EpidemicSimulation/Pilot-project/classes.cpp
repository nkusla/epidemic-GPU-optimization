#include "classes.h"

Person::Person() {
	this->homeID = -1;
	this->workID = -1;
	this->distancing = false;
	this->status = S;
}

Person::Person(int homeID, int workID, bool distancing, Status status){
	this->homeID = homeID;
	this->workID = workID;
	this->distancing = distancing;
	this->status = status;
}

void Person::setHomeID(int homeID) {
	this->homeID = homeID;
}

void Person::setWorkID(int workID) {
	this->workID = workID;
}

void Person::setDistancing(bool distancing) {
	this->distancing = distancing;
}

void Person::setStatus(Status status) {
	this->status = status;
}

void Person::TryInfect(std::default_random_engine generator, int infectionProbability) {
	std::uniform_int_distribution<int> distribution(1, 100);

	int rand = distribution(generator);
	if (rand <= infectionProbability) {
		this->status = I;
	}
}

void Person::TryKill(std::default_random_engine generator, int deathProbability) {
	std::uniform_int_distribution<int> distribution(1, 100);

	int rand = distribution(generator);
	if (rand <= deathProbability) {
		this->status = D;
	}
}

