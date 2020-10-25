// In this header are all functions that are used by main function
// in order to create buffers, build kernels, send data back and forth between
// host and GPU and execute kernels on GPU

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

void BufferDayInfo(std::string& outputHistory, int simulationTime, compute::buffer* numInfectedBuff);

void BufferSimulationEndInfo(std::string& outputHistory, compute::buffer* numInfectedBuff, compute::buffer* numRecoveredBuff,
	compute::buffer* numDeadBuff, compute::buffer* maxInfectedBuff, int executionTime);

void SetDeviceRandGenerators(compute::vector<MTRand>& randGeneratorsDevice, compute::kernel& InitGeneratorsKernel);

void InitData(compute::vector<Person>& peopleDevice, compute::vector<int>& numPeopleOnLocationsDevice, compute::buffer& locationsOnDevice);

void SingleLocationBySingleThread(); // <--- Most important function

