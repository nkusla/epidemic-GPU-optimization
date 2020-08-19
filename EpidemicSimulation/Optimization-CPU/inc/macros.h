// In this header are all necessary macros

#pragma once

// defines how random generator will create random values
#define SEED 1616

// number
#define NUM_PEOPLE 10
#define POPULAR_PLACES 40
#define NUM_HOMES 22000
#define NUM_WORKPLACES 9500
#define NUM_INTERACTIONS 4

// percentage
#define INITIAL_INFECTIONS 0.000125
#define DISTANCING_PERCENTAGE 0.3
#define INFECTION_PROBABILITY 0.00118
#define FATALITY_RATE 0.00405

// days
#define INFECTION_DURATION 12
#define IMMUNITY_DURATION 21
#define SIMULATION_DURATION 45

// hours
#define WORK_HOURS 8
#define LOCATION_DURATION 3
#define DAY_DURATION 18

// Status that shows if person is infected, recovered, dead...
enum Status { S, I, R, D };