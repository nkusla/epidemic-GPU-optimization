#pragma once

#include <iostream>
#include <boost-compute/compute.hpp>
#include <random>
#include <macros.h>
#include <person.h>
#include <func.h>
#include <windows.h>
#include <vector>

namespace compute = boost::compute;

compute::buffer CreateIntBuffer(int value);

compute::buffer InitData(compute::array<Person, NUM_PEOPLE>& peopleDevice);

void SingleLocationBySingleThread();

void SinglePersonBySingleThread();
