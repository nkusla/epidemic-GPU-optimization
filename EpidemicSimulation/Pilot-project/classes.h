// In this file class Person is defined. Every agent in the simulation is instance of class Person

#pragma once
#include <iostream>
#include <random>
#include "macros.h"

class Person
{	
	protected:
		int homeID;
		int workID;
		int currentLocation;
		int infectionDays;
		bool distancing;
		Status status;
	public:
		static int numDead, numInfected, numRecovered;

		Person();
		Person(int homeID, int workID, int currentLocation, int infectionDays, bool distancing, Status status);

		int getHomeID() { return homeID; }
		int getWorkID() { return workID; }
		int getCurrentLocation() { return currentLocation; }
		int getInfectionDays() { return infectionDays; }
		int getDistancing() { return distancing; }
		Status getStatus() { return status; }

		void setHomeID(int homeID);
		void setWorkID(int workID);
		void setCurrentLocation(int currentLocation);
		void setInfectionDays(int infectionDays);
		void setDistancing(bool distancing);
		void setStatus(Status s);

		void ExtendInfectionDay();
		void TryInfect(std::default_random_engine& generator, int infectionProbability);
		bool TryKill(std::default_random_engine& generator, int deathProbability);
		void RecoverAgent();
};
