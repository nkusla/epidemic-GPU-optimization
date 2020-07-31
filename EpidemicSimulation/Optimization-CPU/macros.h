// In this header are all necessary macros

#pragma once

#define SEED 1616

#define NUM_PEOPLE 50000
#define POPULAR_PLACES 25
#define NUM_HOMES 12800
#define NUM_WORKPLACES 6400

// percentage
#define INITIAL_INFECTIONS 0.0001
#define DISTANCING_PERCENTAGE 0.00010
#define INFECTION_PROBABILITY 0.00120
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