// These are some helper functions that are used to generate data and radnomize it...

#pragma once
#include <iostream>
#include <vector>
#include "person.h"

void RandomNoRepetition(std::vector<int>* v, std::default_random_engine& generator);

void SetAgents(Person* people, std::default_random_engine& generator);

void SetAgentsHome(Person* people, std::vector<int>* locations);

void InfectAgents(Person* people, std::default_random_engine& generator);

void RemoveAgentFromCurrentLocation(Person person, int personIndex, std::vector<int>* locations);

void ChangeAgentsLocation(Person* people, std::vector<int>* locations, std::default_random_engine& generator, int dayDuration);

void MakeInteractions(Person* people, std::vector<int>* locations, std::default_random_engine& generator, int locationsSize);

void CheckAgentsHealth(Person* people, std::vector<int>* locations, std::default_random_engine& generator);

void WriteInfo(int simulationTime);

void SimulationEndInfo();
