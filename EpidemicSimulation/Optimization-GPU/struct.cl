#include "mtwister.cl"

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

void TryInfect(__global Person* p, __global MTRand* generator, __global int* INFECTION_DURATION,
    __global int* numInfected, __global int* maxInfected) {

    if(GenerateNumInRange(generator, 1, 100000) <= *INFECTION_DURATION){
        p->status = I;
        if(!p->wasInfected){
            p->wasInfected = true;
            (*numInfected)++;
            (*maxInfected)++;
        }
        else{
            (*numInfected)++;
        }
    }
}

bool TryKill(__global Person* p, __global MTRand* generator, __global int* FATALITY_RATE,
    __global int* numInfected, __global int* numDead) {

    if(GenerateNumInRange(generator, 1, 100000) <= *FATALITY_RATE){
        p->status = D;
        (*numDead)++;
        (*numInfected)--;
        return true;
    } 
    else {
        return false;
    }
}

void RecoverAgent(__global Person* p, __global int* numInfected, __global int* numRecovered){
    p->status = R;
    p->infectionDays = 0;
    (*numRecovered)++;
    (*numInfected)--;
}

void MakeAgentSusceptible(__global Person* p, __global int* numRecovered) {
	p->status = S;
	p->immunityDays = 0;
	(*numRecovered)--;
}