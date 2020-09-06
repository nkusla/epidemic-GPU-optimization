#pragma once

#include <iostream>
#include <random>
#include <boost-compute/compute.hpp>
#include <macros.h>
#include <person.h>
#include <func.h>
#include <windows.h>
#include "mtwister.h"
#include <vector>

namespace compute = boost::compute;

compute::buffer* CreateIntBuffer(int value);

void UpdateIntBuffer(compute::buffer* buff, int value);

void SetDeviceRandGenerators(compute::vector<MTRand>& randGeneratorsDevice, compute::kernel& InitGeneratorsKernel);

void InitData(compute::vector<Person>& peopleDevice, compute::vector<int>& numPeopleOnLocationsDevice, compute::buffer& locationsOnDevice);

void SingleLocationBySingleThread();

void SinglePersonBySingleThread();
