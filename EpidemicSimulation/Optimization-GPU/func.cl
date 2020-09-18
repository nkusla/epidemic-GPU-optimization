#pragma OPENCL EXTENSION cl_khr_global_int32_base_atomics : enable

#include "struct.cl"
#include "mtwister.cl"

__kernel void MakeInteractions(__global int* locations, __global int* width, __global int* numPeopleOnLocations, 
    __global Person* people, __global MTRand* generators, __global int* INFECTION_PROBABILITY,
    __global int* FATALITY_RATE, __global int* numInfected, __global int* maxInfected) {

    int id = get_global_id(0);
    int w = *width;
    int max = numPeopleOnLocations[id];
    
    if (max > 1){
        for(int i = 0; i < max; ++i){
            int personID = locations[id * w + i];
            int rand = GenerateNumInRange(generators + id, 0, max-1);
            int personRandID = locations[id * w + rand];

            if (people[personID].status == I && people[personRandID].status == S) {
                TryInfect(people + personRandID, generators + id, INFECTION_PROBABILITY, numInfected, maxInfected);
            }
            else if (people[personID].status == S && people[personRandID].status == I) {
                TryInfect(people + personID, generators + id, INFECTION_PROBABILITY, numInfected, maxInfected);
            }
            else { continue; }
        }
    }
}

__kernel void CheckAgentsStatus(__global Person* people, __global MTRand* generators, 
    __global int* INFECTION_DURATION, __global int* FATALITY_RATE, __global int* IMMUNITY_DURATION, 
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
        if(people[id].immunityDays == *IMMUNITY_DURATION) {
            MakeAgentSusceptible(people + id, numRecovered);
        }
    }
}

__kernel void ChangeAgentsLocation(__global int* locations, __global int* width, __global Person* people,
    __global int* numPeopleOnLocations, __global MTRand* generators, __global int* dayDuration, 
    __global int* WORK_HOURS, __global int* LOCATION_DURATION,  __global int* NUM_HOMES,  
    __global int* NUM_WORKPLACES, __global int* POPULAR_PLACES){

    int id = get_global_id(0);
    int w = *width;
    int max = numPeopleOnLocations[id];

    if(max > 0){

        const int homeIndexFirst = 0;
        const int homeIndexLast = *NUM_HOMES - 1;

        const int workIndexFirst = homeIndexLast + 1;
        const int workIndexLast = workIndexFirst + *NUM_WORKPLACES - 1;

        const int popularPlaceIndexFirst = workIndexLast + 1;
        const int popularPlaceIndexLast = popularPlaceIndexFirst + *POPULAR_PLACES - 1;

        if (*dayDuration == 0 && id >= homeIndexFirst && id <= homeIndexLast){
            for(int i = 0; i < max; ++i){
                int personID = locations[id * w + i];
                people[personID].currentLocation = people[personID].workID;
            }
        }
        else if (*dayDuration == *WORK_HOURS && id >= workIndexFirst && id <= workIndexLast){
            for(int i = 0; i < max; ++i){
                int personID = locations[id * w + i];
                if(!people[personID].distancing){
                    people[personID].currentLocation = GenerateNumInRange(generators + id, popularPlaceIndexFirst, popularPlaceIndexLast);
                }
                else{
                    people[personID].currentLocation = people[personID].homeID;
                }
            }
        }
        else if (*dayDuration == *WORK_HOURS + *LOCATION_DURATION && id >= popularPlaceIndexFirst && id <= popularPlaceIndexLast){
            for(int i = 0; i < max; ++i){
                int personID = locations[id * w + i];
                people[personID].currentLocation = GenerateNumInRange(generators + id, popularPlaceIndexFirst, popularPlaceIndexLast);
            }
        }
        else if (*dayDuration == *WORK_HOURS + 2 * (*LOCATION_DURATION) && id >= popularPlaceIndexFirst && id <= popularPlaceIndexLast){
            for(int i = 0; i < max; ++i){
                int personID = locations[id * w + i];
                people[personID].currentLocation = people[personID].homeID;
            }
        }
        else{
        }

        numPeopleOnLocations[id] = 0;
        for(int i = 0; i < max; ++i){
            locations[id * w + i] = -1;
        }
    }

}

__kernel void InitGenerators(__global MTRand* generators, __global int* seeds){

    int id = get_global_id(0);
    int s = seeds[id];
    InitSeed(generators + id, s);
}

__kernel void MoveAgentsToLocationsSingleT(__global int* locations, __global int* width, __global int* numPeopleOnLocations,
    __global Person* people, __global int* NUM_PEOPLE){
    
    int w = *width;
    for(int id = 0; id < *NUM_PEOPLE; ++id){
        if(people[id].status != D){
            int locationID = people[id].currentLocation;

            bool hasMoved = false;
            int i = 0;

            while(!hasMoved){
                if(locations[locationID * w + i] == -1){
                    locations[locationID * w + i] = id;
                    numPeopleOnLocations[locationID]++;
                    hasMoved = true;
                }
                else{
                    i++;
                }
            }
        }
    }
}

__kernel void MoveAgentsToLocationsParallel(__global int* locations, __global int* width, __global int* numPeopleOnLocations,
    __global Person* people, __global int* NUM_PEOPLE){
    
    int id = get_global_id(0);
    int w = *width;
    if(people[id].status != D){
        int locationID = people[id].currentLocation;

        bool hasMoved = false;
        int i = 0;

        while(!hasMoved){
            int oldValue = atomic_cmpxchg(locations + (locationID * w + i), -1, id);
            if(oldValue == -1){
                atomic_inc(numPeopleOnLocations + locationID);
                hasMoved = true;
            }
            else{
                i++;
            }
        }
    }
}

__kernel void UpdateDayDuration(__global int* dayDuration, __global int* DAY_DURATION){
    
    if(*dayDuration == *DAY_DURATION){
        *dayDuration = 0;
    }
    else{
        (*dayDuration)++;
    }
}
