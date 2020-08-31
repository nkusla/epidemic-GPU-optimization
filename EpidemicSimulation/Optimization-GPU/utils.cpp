#include "utils.h"

const int mainSize = NUM_HOMES + NUM_WORKPLACES + POPULAR_PLACES;
const int maxLocationSize = 821;
int locationsOnHost[mainSize * maxLocationSize];

compute::device gpu = compute::system::default_device();
compute::context context(gpu);
compute::command_queue queue(context, gpu);

compute::buffer CreateIntBuffer(int value) {
	compute::buffer buf(context, sizeof(int));
	queue.enqueue_write_buffer(buf, 0, sizeof(int), &value);
	queue.finish();

	return buf;
}

compute::buffer InitData(compute::array<Person, NUM_PEOPLE> peopleDevice) {

	Person people[NUM_PEOPLE];
	std::vector<int> locations[mainSize];

	std::default_random_engine generator(SEED);
	InitiateAgents(people, generator);
	SetAgentsHome(people, locations);
	InfectAgents(people, generator);

	compute::copy(people, people + NUM_PEOPLE, peopleDevice.begin(), queue);
	queue.finish();

	for (int i = 0; i < mainSize * maxLocationSize; ++i) {
		locationsOnHost[i] = -1;
	}

	for (int i = 0; i < mainSize; ++i) {
		for (int j = 0; j < locations[i].size(); ++j) {
			locationsOnHost[i * maxLocationSize + j] = locations[i][j];
		}
	}

	compute::buffer buf(context, sizeof(int) * maxLocationSize * mainSize);
	queue.enqueue_write_buffer(buf, 0, maxLocationSize * mainSize * sizeof(int), locationsOnHost);
	queue.finish();

	return buf;
}

void SingleLocationBySingleThread() {
	
	compute::array<Person, NUM_PEOPLE> peopleDevice(context);
	compute::buffer locationsBuffer = InitData(peopleDevice);
	
	compute::program funcProgram = compute::program::create_with_source_file("func.cl", context);
	funcProgram.build();
	compute::kernel funcKernel = funcProgram.create_kernel("func");

	funcKernel.set_arg(0, sizeof(locationsBuffer), &locationsBuffer);
	compute::buffer buf = CreateIntBuffer(maxLocationSize);
	funcKernel.set_arg(1, sizeof(buf), &buf);

	size_t global_dimensions[] = { mainSize, 0, 0 };
	size_t work_dim = 1;
	queue.enqueue_nd_range_kernel(funcKernel, work_dim, NULL, global_dimensions, NULL);
	queue.finish();
}