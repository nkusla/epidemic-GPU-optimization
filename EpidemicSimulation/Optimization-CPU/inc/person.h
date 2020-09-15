// In this file class Person is defined. Every agent in the simulation is instance of class Person
// Class GlobalVars stores paths and vector that saves random numbers that were generated

#pragma once
#include <iostream>
#include <random>
#include "macros.h"

// Status that shows if person is infected, recovered, dead...
enum Status { S, I, R, D };

class Person
{	
	protected:
		int homeID;
		int workID;
		int currentLocation;
		int infectionDays; // Tracks how long person has been infected
		int immunityDays; // Tracks how long person has been immune
		bool wasInfected; // Determines if person was infected at least once
		bool distancing; // Determines if person is social distancing
		Status status;
	public:
		static int numDead, numInfected, numRecovered; // Number of dead, infected and recovered people at the moment
		static int maxInfected; // Number of people who have been infected at least once since the start of an epidemic
		static int newInfected; // Number of infected in the same day (new cases)
		static int maxNewInfected; // Max number of infected on the same days (max new cases)

		Person();
		Person(int homeID, int workID, int currentLocation, int infectionDays, bool immunityDays,
			bool wasInfected, bool distancing, Status status);

		int getHomeID() { return homeID; }
		int getWorkID() { return workID; }
		int getCurrentLocation() { return currentLocation; }
		int getInfectionDays() { return infectionDays; }
		int getImmunityDays() { return immunityDays; }
		bool getWasInfected() { return wasInfected; }
		bool getDistancing() { return distancing; }
		Status getStatus() { return status; }

		void setHomeID(int homeID);
		void setWorkID(int workID);
		void setCurrentLocation(int currentLocation);
		void setInfectionDays(int infectionDays);
		void setImmunityDays(int immunityDays);
		void setWasInfected(bool wasInfected);
		void setDistancing(bool distancing);
		void setStatus(Status s);

		void ExtendInfectionDay();
		void ExtendImmunityDay();
		bool TryInfect(std::default_random_engine& generator, int infectionProbability);
		bool TryKill(std::default_random_engine& generator, int deathProbability);
		void RecoverAgent();
		void MakeAgentSusceptible();

		static void changeMaxNewInfected();
};
