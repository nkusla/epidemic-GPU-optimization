// In this header are all necessary macros

#pragma once

#define SEED 1616

#define NUM_PEOPLE 10000
#define POPULAR_PLACES 150
#define NUM_HOMES 3000
#define NUM_WORKPLACES 1000

// percentage
#define INITIAL_INFECTIONS 0.001
#define DISTANCING_PERCENTAGE 0.4
#define INFECTION_PROBABILITY 0.4
#define FATALITY_RATE 0.2

// days
#define INFECTION_DURATION 12
#define SIMULATION_DURATION 50

// hours
#define WORK_HOURS 9
#define LOCATION_DURATION 2
#define DAY_DURATION 15

#define NUM_INTERACTIONS 4

// Status that show if person is infected, recovered, dead...
enum Status { S, I, R, D };