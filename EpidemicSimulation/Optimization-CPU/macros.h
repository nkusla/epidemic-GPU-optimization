// In this header are all necessary macros

#pragma once

#define SEED 1616

#define NUM_PEOPLE 10000
#define POPULAR_PLACES 20
#define NUM_HOMES 2650
#define NUM_WORKPLACES 1100

// percentage
#define INITIAL_INFECTIONS 0.0005
#define DISTANCING_PERCENTAGE 0.3
#define INFECTION_PROBABILITY 0.00118
#define FATALITY_RATE 0.00025

// days
#define INFECTION_DURATION 12
#define SIMULATION_DURATION 45

// hours
#define WORK_HOURS 8
#define LOCATION_DURATION 3
#define DAY_DURATION 18

#define NUM_INTERACTIONS 4

// Status that show if person is infected, recovered, dead...
enum Status { S, I, R, D };