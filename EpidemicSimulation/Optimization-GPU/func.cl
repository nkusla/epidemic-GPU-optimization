#include "struct.cl"
#include "mtwister.cl"

__kernel void MakeInteractions(__global int* locations, __global int* width, __global int* numPeopleOnLocations, 
    __global Person* people, __global MTRand* generators, __global int* INFECTION_PROBABILITY,
    __global int* FATALITY_RATE, __global int* numInfected, __global int* maxInfected) {

    int id = get_global_id(0);
    int w = *width;
    int max = numPeopleOnLocations[id];

    for(int i = 0; i < max; ++i){
        int personID = locations[id * w + i];
        int personRandID = GenerateNumInRange(generators + id, 1, max);

        if (people[personID].status == I && people[personRandID].status == S) {
            TryInfect(people + personRandID, generators + id, INFECTION_PROBABILITY, numInfected, maxInfected);
        }
        else if (people[personID].status == S && people[personRandID].status == I) {
            TryInfect(people + personID, generators + id, INFECTION_PROBABILITY, numInfected, maxInfected);
        }
        else { continue; }
    }   
}

__kernel void CheckAgentsStatus(__global Person* people, __global MTRand* generators, 
    __global int* FATALITY_RATE, __global int* INFECTION_DURATION, __global int* IMMUNITY_DURATION, 
    __global int* numInfected, __global int* numRecovered, __global int* numDead) {
    
    int id = get_global_id(0);

    if(people[id].status == I){
        bool isKilled = TryKill(people + id, generators + id, FATALITY_RATE, numInfected, numDead);
        if(!isKilled){
            people[id].infectionDays++;
            if(people[id].infectionDays == *INFECTION_DURATION){
                RecoverAgent(people + id, numInfected, numRecovered);
            }
        }
    }
    else if(people[id].status == R){
        people[id].immunityDays++;
        if(people[id].infectionDays == *IMMUNITY_DURATION) {
            MakeAgentSusceptible(people + id, numRecovered);
        }
    }
}

__kernel void RemoveAllAgentsFromLocations(__global int* locations, __global int* width, 
    __global int* numPeopleOnLocations){

    int id = get_global_id(0);
    int w = *width;
    int max = numPeopleOnLocations[id];
    for(int i = 0; i < max; ++i){
        locations[id * w + i] = -1;
    }
}

__kernel void InitGenerators(__global MTRand* generators, __global int* seeds){

    int id = get_global_id(0);
    int s = seeds[id];
    InitSeed(generators + id, s);
}