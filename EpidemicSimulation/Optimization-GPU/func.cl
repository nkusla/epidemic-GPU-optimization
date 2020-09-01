#include "struct.cl"
#include "mtwister.cl"

__kernel void MakeInteractions(__global int* locations, __global int* width, __global int* numPeople, 
    __global Person* people, __global MTRand* generators, __global int* infectionProbability) {

    int id = get_global_id(0);
    int w = *width;

    for(int i = 0; i < w; ++i){
        int personID = locations[id * w + i];
        if(personID != -1){   
            int personRandID = GenerateNumInRange(generators + id, 1, *numPeople);

            if (people[personID].status == I && people[personRandID].status == S) {
                TryInfect(people + personRandID, generators + id, infectionProbability);
            }
            else if (people[personID].status == S && people[personRandID].status == I) {
                TryInfect(people + personID, generators + id, infectionProbability);
            }
            else { continue; }
        }
        else{
            break;
        }     
    }
}

__kernel void CheckAgentsStatus(__global int* locations, __global int* width, 
    __global Person* people, __global MTRand* generators, __global int* deathProbability) {

}