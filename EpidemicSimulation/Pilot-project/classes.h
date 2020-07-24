// In this file class Person is defined. Every agent in the simulation is instance of class Person

#pragma once
#include <random>
#include "macros.h"

class Person
{	
	protected:
		int homeID;
		int workID;
		bool distancing;
		Status status;
	public:
		Person();
		Person(int homeID, int workID, bool distancing, Status status);

		int getHomeID() { return homeID; }
		int getWorkID() { return workID; }
		int getDistancing() { return distancing; }
		Status getStatus() { return status; }

		void setHomeID(int homeID);
		void setWorkID(int workID);
		void setDistancing(bool distancing);
		void setStatus(Status s);

		void TryInfect(std::default_random_engine generator, int infectionProbability);
		void TryKill(std::default_random_engine generator, int deathProbability);
};

