#include "macros.h"

class Person
{	
	protected:
		int homeID;
		bool distancing;
		Status status;
	public:
		Person(int homeID, bool distancing, Status status);

		int getHomeID() { return homeID; }
		int getDistancing() { return distancing; }
		Status getStatus() { return status; }

		void setHomeID(int homeID);
		void setDistancing(bool distancing);
		void setStatus(Status s);
};

