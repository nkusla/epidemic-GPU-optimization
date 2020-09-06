#include "utils.h"

// Global variables ----------------------------------------------

const int mainSize = NUM_HOMES + NUM_WORKPLACES + POPULAR_PLACES;
const int maxLocationSize = 3000;

int locationsHost[mainSize * maxLocationSize];
Person people[NUM_PEOPLE];
std::vector<int> locations[mainSize];

compute::device gpu = compute::system::default_device();
compute::context context(gpu);
compute::command_queue queue(context, gpu);

// ---------------------------------------------------------------

compute::buffer* CreateIntBuffer(int value) {

	compute::buffer* buff = new compute::buffer(context, sizeof(int));
	queue.enqueue_write_buffer(*buff, 0, sizeof(int), &value);
	queue.finish();

	return buff;
}

void UpdateIntBuffer(compute::buffer* buff, int value) {
	queue.enqueue_write_buffer(*buff, 0, sizeof(int), &value);
	queue.finish();
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

void InitData(compute::vector<Person>& peopleDevice, compute::vector<int>& numPeopleOnLocationsDevice, compute::buffer& locationsOnDevice) {
	
	std::default_random_engine generator(SEED);
	std::vector<int> numPeopleOnLocations;
	InitiateAgents(people, generator);
	SetAgentsHome(people, locations);
	InfectAgents(people, generator);

	for (int i = 0; i < mainSize; ++i) {
		numPeopleOnLocations.push_back(0);
	}

	for (int i = 0; i < mainSize * maxLocationSize; ++i) {
		locationsHost[i] = -1;
	}

	for (int i = 0; i < mainSize; ++i) {
		for (int j = 0; j < locations[i].size(); ++j) {
			locationsHost[i * maxLocationSize + j] = locations[i][j];
			numPeopleOnLocations[i]++;
		}
	}

	compute::copy(people, people + NUM_PEOPLE, peopleDevice.begin(), queue);
	queue.finish();
	compute::copy(numPeopleOnLocations.begin(), numPeopleOnLocations.end(), numPeopleOnLocationsDevice.begin(), queue);
	queue.finish();

	queue.enqueue_write_buffer(locationsOnDevice, 0, sizeof(int) * maxLocationSize * mainSize, locationsHost);
	queue.finish();
}

void SingleLocationBySingleThread() {
	
	// ----- Main variables and data -----------------------------------------------------------------

	compute::vector<Person> peopleDevice(NUM_PEOPLE, context);
	compute::buffer locationsOnDevice(context, sizeof(int) * maxLocationSize * mainSize);
	compute::vector<int> numPeopleOnLocationsDevice(mainSize, context);
	compute::vector<MTRand> randGeneratorsDevice(NUM_PEOPLE, context);
	
	compute::buffer* maxLocationSizeBuff = CreateIntBuffer(maxLocationSize);

	compute::buffer* NUM_PEOPLE_Buff = CreateIntBuffer(NUM_PEOPLE);
	compute::buffer* NUM_HOMES_Buff = CreateIntBuffer(NUM_HOMES);
	compute::buffer* NUM_WORKPLACES_Buff = CreateIntBuffer(NUM_WORKPLACES);
	compute::buffer* POPULAR_PLACES_Buff = CreateIntBuffer(POPULAR_PLACES);
	compute::buffer* WORK_HOURS_Buff = CreateIntBuffer(WORK_HOURS);
	compute::buffer* LOCATION_DURATION_Buff = CreateIntBuffer(LOCATION_DURATION);

	compute::buffer* INFECTION_PROBABILITY_Buff = CreateIntBuffer(INFECTION_PROBABILITY * 10000);
	compute::buffer* FATALITY_RATE_Buff = CreateIntBuffer(FATALITY_RATE * 10000);
	compute::buffer* IMMUNITY_DURATION_Buff = CreateIntBuffer(IMMUNITY_DURATION);

	compute::buffer* numInfectedBuff = CreateIntBuffer(Person::numInfected);
	compute::buffer* numRecoveredBuff = CreateIntBuffer(Person::numRecovered);
	compute::buffer* numDeadBuff = CreateIntBuffer(Person::numDead);
	compute::buffer* maxInfectedBuff = CreateIntBuffer(Person::maxInfected);

	size_t work_dim = 1;
	size_t global_dimensions[] = { 1, 0, 0 };

	int i = 0, simulationTime = 0, dayDuration = 0;
	compute::buffer* dayDurationBuff = CreateIntBuffer(dayDuration);

	// ----- Preparing data and kernels ---------------------------------------------------------------

	InitData(peopleDevice, numPeopleOnLocationsDevice, locationsOnDevice);

	compute::program funcProgram = compute::program::create_with_source_file("func.cl", context);

	try {
		funcProgram.build("-I .");
	}
	catch (boost::compute::opencl_error& e) {
		std::cout << funcProgram.build_log() << std::endl;
		std::cout << std::endl;
	}

	// Setting kernels

	compute::kernel InitGeneratorsKernel = funcProgram.create_kernel("InitGenerators");
	SetDeviceRandGenerators(randGeneratorsDevice, InitGeneratorsKernel);

	compute::kernel MakeInteractionsKernel = funcProgram.create_kernel("MakeInteractions");
	MakeInteractionsKernel.set_arg(0, sizeof(locationsOnDevice), &locationsOnDevice);
	MakeInteractionsKernel.set_arg(1, sizeof(*maxLocationSizeBuff), maxLocationSizeBuff);
	MakeInteractionsKernel.set_arg(2, numPeopleOnLocationsDevice);
	MakeInteractionsKernel.set_arg(3, peopleDevice);
	MakeInteractionsKernel.set_arg(4, randGeneratorsDevice);
	MakeInteractionsKernel.set_arg(5, sizeof(*INFECTION_PROBABILITY_Buff), INFECTION_PROBABILITY_Buff);
	MakeInteractionsKernel.set_arg(6, sizeof(*FATALITY_RATE_Buff), FATALITY_RATE_Buff);
	MakeInteractionsKernel.set_arg(7, sizeof(*numInfectedBuff), numInfectedBuff);
	MakeInteractionsKernel.set_arg(8, sizeof(*maxInfectedBuff), maxInfectedBuff);

	compute::kernel CheckAgentsStatusKernel = funcProgram.create_kernel("CheckAgentsStatus");
	CheckAgentsStatusKernel.set_arg(0, peopleDevice);
	CheckAgentsStatusKernel.set_arg(1, randGeneratorsDevice);
	CheckAgentsStatusKernel.set_arg(2, sizeof(*INFECTION_PROBABILITY_Buff), INFECTION_PROBABILITY_Buff);
	CheckAgentsStatusKernel.set_arg(3, sizeof(*FATALITY_RATE_Buff), FATALITY_RATE_Buff);
	CheckAgentsStatusKernel.set_arg(4, sizeof(*IMMUNITY_DURATION_Buff), IMMUNITY_DURATION_Buff);
	CheckAgentsStatusKernel.set_arg(5, sizeof(*numInfectedBuff), numInfectedBuff);
	CheckAgentsStatusKernel.set_arg(6, sizeof(*numRecoveredBuff), numRecoveredBuff);
	CheckAgentsStatusKernel.set_arg(7, sizeof(*numDeadBuff), numDeadBuff);
	
	compute::kernel ChangeAgentsLocationKernel = funcProgram.create_kernel("ChangeAgentsLocation");
	ChangeAgentsLocationKernel.set_arg(0, sizeof(locationsOnDevice), &locationsOnDevice);
	ChangeAgentsLocationKernel.set_arg(1, sizeof(*maxLocationSizeBuff), maxLocationSizeBuff);
	ChangeAgentsLocationKernel.set_arg(2, peopleDevice);
	ChangeAgentsLocationKernel.set_arg(3, numPeopleOnLocationsDevice);
	ChangeAgentsLocationKernel.set_arg(4, randGeneratorsDevice);
	ChangeAgentsLocationKernel.set_arg(5, sizeof(*dayDurationBuff), dayDurationBuff);
	ChangeAgentsLocationKernel.set_arg(6, sizeof(*WORK_HOURS_Buff), WORK_HOURS_Buff);
	ChangeAgentsLocationKernel.set_arg(7, sizeof(*LOCATION_DURATION_Buff), LOCATION_DURATION_Buff);
	ChangeAgentsLocationKernel.set_arg(8, sizeof(*NUM_HOMES_Buff), NUM_HOMES_Buff);
	ChangeAgentsLocationKernel.set_arg(9, sizeof(*NUM_WORKPLACES_Buff), NUM_WORKPLACES_Buff);
	ChangeAgentsLocationKernel.set_arg(10, sizeof(*POPULAR_PLACES_Buff), POPULAR_PLACES_Buff);

	compute::kernel MoveAgentsToLocationsKernel = funcProgram.create_kernel("MoveAgentsToLocations");
	MoveAgentsToLocationsKernel.set_arg(0, sizeof(locationsOnDevice), &locationsOnDevice);
	MoveAgentsToLocationsKernel.set_arg(1, sizeof(*maxLocationSizeBuff), maxLocationSizeBuff);
	MoveAgentsToLocationsKernel.set_arg(2, numPeopleOnLocationsDevice);
	MoveAgentsToLocationsKernel.set_arg(3, peopleDevice);
	MoveAgentsToLocationsKernel.set_arg(4, sizeof(*NUM_PEOPLE_Buff), NUM_PEOPLE_Buff);

	// ----- Main -------------------------------------------------------------------------------------

	auto start = std::chrono::high_resolution_clock::now();

	while (simulationTime < SIMULATION_DURATION * DAY_DURATION) {
		
		global_dimensions[0] = mainSize;
		queue.enqueue_nd_range_kernel(ChangeAgentsLocationKernel, work_dim, NULL, global_dimensions, NULL);
		queue.finish();

		queue.enqueue_task(MoveAgentsToLocationsKernel);
		queue.finish();

		while (i < NUM_INTERACTIONS) {
			global_dimensions[0] = mainSize;
			queue.enqueue_nd_range_kernel(MakeInteractionsKernel, work_dim, NULL, global_dimensions, NULL);
			queue.finish();
			++i;
		}
		i = 0;
		dayDuration += 1;
		UpdateIntBuffer(dayDurationBuff, dayDuration);

		if (dayDuration == DAY_DURATION) {
			simulationTime += dayDuration;
			dayDuration = 0;
			UpdateIntBuffer(dayDurationBuff, dayDuration);

			global_dimensions[0] = NUM_PEOPLE;
			queue.enqueue_nd_range_kernel(CheckAgentsStatusKernel, work_dim, NULL, global_dimensions, NULL);
			queue.finish();
			std::cout << "Day " << simulationTime / DAY_DURATION << std::endl;
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	int executionTime = duration.count();

	std::cout << "Time: " << executionTime << " ms" << std::endl;
}