// In this header are all necessary macros
#pragma once

#define SEED 1616;

#define NUM_PEOPLE 10000;
#define POPULAR_PLACES 30;
#define NUM_HOMES 3300;
#define NUM_WORKPLACES 2000;

// percentage
#define INITIAL_INFECTIONS 0.01;
#define DISTANCING_PERCENTAGE 0.2;
#define INFECTION_PROBABILITY 0.4;
#define HOME_INFECTION_PROBABILITY 0.6;
#define FATALITY_RATE 0.01;
#define SATURATED_FATALITY_RATE 0.07;

// days
#define INFECTION_DURATION 12;

// hours
#define WORK_HOURS 9;
#define DAY_DURATION 14;

// Status that show if person is infected, recovered, dead...
enum Status { S, I, R, D };