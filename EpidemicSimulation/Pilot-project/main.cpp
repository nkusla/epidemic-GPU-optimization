#include <iostream>
#include "classes.h"
#include "func.h"

Person people[NUM_PEOPLE];
std::default_random_engine generator;

int main()
{
	generator.seed(SEED);
	SetAgents(people, generator);

	std::cout << "Hello world!" << std::endl;
}