#define SIZE 10

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <CL/opencl.h>

using namespace std;

cl_float* createDataArray(){
	cl_float* data = new cl_float[SIZE];
	for (int i = 0; i < SIZE; ++i) {
		data[i] = (float)i/2 * CL_M_PI;
	}
	return data;
}

void writeDataArray(cl_float* data){
	for (int i = 0; i < SIZE; ++i) {
		cout << data[i] << endl;
	}
	cout << endl;
}

int main()
{
	cl_device_id device;
	cl_platform_id platform;
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_kernel kernel;
	cl_mem buffer;
	
	cl_float* data = createDataArray();
	size_t dataSize = sizeof(cl_float)*SIZE;
	writeDataArray(data);

	const char* source = {
		"__kernel void calcSin(__global float* data) {\n"
		"	int id = get_global_id(0);\n"
		"	data[id] = sin(data[id]);\n"
		"}\n"
	};
	size_t sourceLength = strlen(source);

	cl_int* errcode = new cl_int;

	clGetPlatformIDs(1, &platform, NULL);
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
	queue = clCreateCommandQueue(context, device, 0, NULL);
	
	program = clCreateProgramWithSource(context, 1, (const char **)&source, &sourceLength, NULL);
	clBuildProgram(program, 1, &device, NULL, NULL, NULL);
	kernel = clCreateKernel(program, "calcSin", errcode);

	buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, dataSize, NULL, NULL);

	clEnqueueWriteBuffer(queue, buffer, CL_TRUE, 0, dataSize, data, 0, NULL, NULL);

	clSetKernelArg(kernel, 0, sizeof(buffer), &buffer);
	size_t global_dimensions[] = { SIZE, 0, 0 };
	clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_dimensions, NULL, 0, NULL, NULL);

	clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, sizeof(cl_float) * SIZE, data, 0, NULL, NULL);

	clFinish(queue);

	writeDataArray(data);
}
