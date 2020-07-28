// In this header are all necessary macros

#pragma once

#define SEED 1616

#define NUM_PEOPLE 300000
#define POPULAR_PLACES 65
#define NUM_HOMES 80000
#define NUM_WORKPLACES 28000

// percentage
#define INITIAL_INFECTIONS 0.000003
#define DISTANCING_PERCENTAGE 0.0001
#define INFECTION_PROBABILITY 0.0015
#define FATALITY_RATE 0

// days
#define INFECTION_DURATION 13
#define SIMULATION_DURATION 96

// hours
#define WORK_HOURS 8
#define LOCATION_DURATION 2
#define DAY_DURATION 15

#define NUM_INTERACTIONS 4

// Status that show if person is infected, recovered, dead...
enum Status { S, I, R, D };