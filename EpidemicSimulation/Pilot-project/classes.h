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
		int infectionDays; // Tracks how long person has been infected
		bool distancing; // Determines if person is social distancing
		Status status;
	public:
		static int numDead, numInfected, numRecovered; // Number of dead, infected and recovered people at the momenz
		static int maxInfected; // Number of infected people since the start of an epidemic
		static int newInfected; // Number of infected in the same day (new cases)
		static int maxNewInfected; // Max number of infected on the same days (max new cases)

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
		bool TryInfect(std::default_random_engine& generator, int infectionProbability);
		bool TryKill(std::default_random_engine& generator, int deathProbability);
		void RecoverAgent();

		static void changeMaxNewInfected();
};
