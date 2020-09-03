#include "utils.h"

// Global variables ----------------------------------------------

const int mainSize = NUM_HOMES + NUM_WORKPLACES + POPULAR_PLACES;
const int maxLocationSize = 1000;

int locationsHost[mainSize * maxLocationSize];
Person people[NUM_PEOPLE];
std::vector<int> locations[mainSize];

compute::device gpu = compute::system::default_device();
compute::context context(gpu);
compute::command_queue queue(context, gpu);

// ---------------------------------------------------------------

compute::buffer CreateIntBuffer(int value) {
	compute::buffer buf(context, sizeof(int));
	queue.enqueue_write_buffer(buf, 0, sizeof(int), &value);
	queue.finish();

	return buf;
}

void SetDeviceRandGenerators(compute::vector<MTRand>& randGeneratorsDevice, compute::kernel& InitGeneratorsKernel) {

	std::vector<int> seedsHost;
	compute::vector<int> seedsDevice(NUM_PEOPLE, context);
	for (int i = 0; i < NUM_PEOPLE; ++i) {
		seedsHost.push_back(i * 5);
	}

	compute::copy(seedsHost.begin(), seedsHost.end(), seedsDevice.begin(), queue);
	queue.finish();

	InitGeneratorsKernel.set_arg(0, randGeneratorsDevice);
	InitGeneratorsKernel.set_arg(1, seedsDevice);

	size_t global_dimensions[] = { NUM_PEOPLE, 0, 0 };
	size_t work_dim = 1;
	queue.enqueue_nd_range_kernel(InitGeneratorsKernel, work_dim, NULL, global_dimensions, NULL);
	queue.finish();

}

void InitData(compute::vector<Person>& peopleDevice, compute::buffer& locationsDevice) {
	
	std::default_random_engine generator(SEED);
	InitiateAgents(people, generator);
	SetAgentsHome(people, locations);
	InfectAgents(people, generator);

	compute::copy(people, people + NUM_PEOPLE, peopleDevice.begin(), queue);
	queue.finish();

	for (int i = 0; i < mainSize * maxLocationSize; ++i) {
		locationsHost[i] = -1;
	}

	for (int i = 0; i < mainSize; ++i) {
		for (int j = 0; j < locations[i].size(); ++j) {
			locationsHost[i * maxLocationSize + j] = locations[i][j];
		}
	}

	queue.enqueue_write_buffer(locationsDevice, 0, sizeof(int) * maxLocationSize * mainSize, locationsHost);
	queue.finish();
}

void SingleLocationBySingleThread() {
	
	compute::vector<Person> peopleDevice(NUM_PEOPLE, context);
	compute::buffer locationsOnDevice(context, sizeof(int) * maxLocationSize * mainSize);
	compute::vector<MTRand> randGeneratorsDevice(NUM_PEOPLE, context);

	InitData(peopleDevice, locationsOnDevice);

	compute::program funcProgram = compute::program::create_with_source_file("func.cl", context);

	try {
		funcProgram.build("-I .");
	}
	catch (boost::compute::opencl_error& e) {
		std::cout << funcProgram.build_log() << std::endl;
		std::cout << std::endl;
	}

	compute::kernel InitGeneratorsKernel = funcProgram.create_kernel("InitGenerators");
	SetDeviceRandGenerators(randGeneratorsDevice, InitGeneratorsKernel);
	
	//compute::kernel funcKernel = funcProgram.create_kernel("GenerateRandInRange");

	/*int randPerson[] = { 5, 6, 7, 2, 4, 8, 1, 7, 3, 9 };
	compute::buffer randPersonBuf(context, sizeof(int) * 10);
	queue.enqueue_write_buffer(randPersonBuf, 0, sizeof(int) * 10, &randPerson);
	queue.finish();

	int randInfection[] = { 6, 6, 6, 6, 6, 6, 6, 6, 8, 9, 10, 11, 12 };
	compute::buffer randInfectionBuf(context, sizeof(int) * 13);
	queue.enqueue_write_buffer(randInfectionBuf, 0, sizeof(int) * 13, &randInfection);
	queue.finish();

	int rand = 5;
	compute::buffer randBuf(context, sizeof(int));
	queue.enqueue_write_buffer(randBuf, 0, sizeof(int), &rand);
	queue.finish();

	funcKernel.set_arg(0, sizeof(locationsBuffer), &locationsBuffer);
	compute::buffer buf = CreateIntBuffer(maxLocationSize);
	funcKernel.set_arg(1, sizeof(buf), &buf);
	funcKernel.set_arg(2, peopleOnDevice);
	funcKernel.set_arg(3, sizeof(randPersonBuf), &randPersonBuf);
	funcKernel.set_arg(4, sizeof(randInfectionBuf), &randInfectionBuf);
	funcKernel.set_arg(5, sizeof(randBuf), &randBuf);*/

	/*size_t global_dimensions[] = { mainSize, 0, 0 };
	size_t work_dim = 1;
	queue.enqueue_nd_range_kernel(funcKernel, work_dim, NULL, global_dimensions, NULL);
	queue.finish();

	queue.enqueue_read_buffer(locationsOnDevice, 0, sizeof(int) * mainSize * maxLocationSize, locationsOnHost);
	queue.finish();*/
}