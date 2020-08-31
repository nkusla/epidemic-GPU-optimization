enum Status { S, I, R, D };

typedef struct {
    int homeID;
    int workID;
    int currentLocation;
    int infectionDays;
    int immunityDays;
    bool wasInfected;
    bool distancing;
    enum Status status;
} Person;
