#include "classes.h"

Person::Person(int homeID, bool distancing, Status status){
	this->homeID = homeID;
	this->distancing = distancing;
	this->status = status;
}

void Person::setHomeID(int homeID) {
	this->homeID = homeID;
}

void Person::setDistancing(bool distancing) {
	this->distancing = distancing;
}

void Person::setStatus(Status status) {
	this->status = status;
}

