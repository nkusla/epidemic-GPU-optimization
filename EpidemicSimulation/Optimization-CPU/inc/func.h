// These are some helper functions that are used to generate data and radnomize it...

#pragma once
#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include "person.h"

// ------------------------------------------------------------------------------------------------------

void RandomNoRepetition(std::vector<int>& v, std::default_random_engine& generator);

void InitiateAgents(Person* people, std::default_random_engine& generator);

void SetAgentsHome(Person* people, std::vector<int>* locations);

void InfectAgents(Person* people, std::default_random_engine& generator);

// ------------------------------------------------------------------------------------------------------

void RemoveAgentFromCurrentLocation(Person person, int personIndex, std::vector<int>* locations);

void MakeInteractions(Person* people, std::vector<int>* locations, std::default_random_engine& generator, int locationsSize);

void ChangeAgentsLocation(Person* people, std::vector<int>* locations, std::default_random_engine& generator, int dayDuration);

void CheckAgentsStatus(Person* people, std::vector<int>* locations, std::default_random_engine& generator);

// ------------------------------------------------------------------------------------------------------

std::string GetCurrentDate();

void WriteInfo(int simulationTime, std::string& outputHistory);

void SimulationEndInfo(std::string& outputHistory, int executionTime);

void LogSimulationParameters(std::string& outputHistory);
