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

bool TryInfect(__global Person* p, __global MTRand* generator, __global int* infectionProbability) {

    if(GenerateNumInRange(generator, 1, 100000) <= *infectionProbability){
        if(!p->wasInfected){
            p->status = I;
            p->wasInfected = true;
            return true;
        }
    }
    else{
        return false;
    }
}

bool TryKill(__global Person* p, __global MTRand* generator, __global int* deathProbability) {

    if(GenerateNumInRange(generator, 1, 100000) <= *deathProbability){
        if(!p->wasInfected){
            p->status = D;
            p->wasInfected = true;
            return true;
        }
    }
    else{
        return false;
    }
}
