// These are some helper functions that are used to generate data and radnomize it...

#pragma once
#include "classes.h"

void ShuffleVector(std::vector<int>* p);

void SetVector(std::vector<int>* p);

void SetAgents(Person* people, std::default_random_engine generator);