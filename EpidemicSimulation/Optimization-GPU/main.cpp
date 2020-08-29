#include <iostream>
#include <boost-compute/compute.hpp>
#include <vector>
//#include <boost/compute/core.hpp>

#define SIZE 10

namespace compute = boost::compute;

int main()
{
	compute::device gpu = compute::system::default_device();
	std::vector<int> host_vector;
	for (int i = 0; i < SIZE; ++i) {
		host_vector.push_back(69);
	}

	compute::context ctx(gpu);
	compute::vector<int> device_vector(SIZE, ctx);
	compute::command_queue queue(ctx, gpu);

	compute::copy(host_vector.begin(), host_vector.end(), device_vector.begin(), queue);
	compute::program testProgram = compute::program::create_with_source_file("test.cl", ctx);

	testProgram.build();
	compute::kernel testKernel = testProgram.create_kernel("test");

	testKernel.set_arg(0, device_vector);
	size_t global_dimensions[] = { SIZE, 0, 0 };
	size_t work_dim = 1;
	queue.enqueue_nd_range_kernel(testKernel, work_dim, NULL, global_dimensions, NULL);
	clFinish(queue);

	compute::copy(device_vector.begin(), device_vector.end(), host_vector.begin(), queue);

	for (int i = 0; i < SIZE; ++i) {
		std::cout << host_vector[i] << std::endl;
	}

	return 0;
}