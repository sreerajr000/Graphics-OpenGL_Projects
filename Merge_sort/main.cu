/*
 * main.cu
 *
 *  Created on: 12-Sep-2017
 *      Author: sree
 */

#include "util.h"
#include "help.h"

// gets called by gpu_mergesort() for each slice
__device__ void gpu_merge(int* src, int* dst, int start,
		int middle, int end) {
	int i = start;
	int j = middle;
	for (int k = start; k < end; k++) {
		if (i < middle && (j >= end || src[i] < src[j])) {
			dst[k] = src[i];
			i++;
		} else {
			dst[k] = src[j];
			j++;
		}
	}
}

//mergesort on our section of the data.
__global__ void gpu_mergesort(int* source, int* dest, int size, int width,
		int slices, dim3* threads, dim3* blocks) {
	unsigned int idx = getIdx(threads, blocks);
	int start = width * idx * slices, middle, end;

	for (int slice = 0; slice < slices; slice++) {
		if (start >= size)
			break;

		middle = min(start + (width >> 1), size);
		end = min(start + width, size);
		gpu_merge(source, dest, start, middle, end);
		start += width;
	}
}

void mergesort(int* data, int size, dim3 threadsPerBlock,
		dim3 blocksPerGrid) {
	int* device_data;
	int* device_swp;
	dim3* device_threads;
	dim3* device_blocks;

	// allocate on gpu
	tm();
	checkCudaErrors(cudaMalloc((void** ) &device_data, size * sizeof(int)));
	checkCudaErrors(cudaMalloc((void** ) &device_swp, size * sizeof(int)));

	std::cout << "cudaMalloc device lists: " << tm() << " microseconds\n";

	checkCudaErrors(
			cudaMemcpy(device_data, data, size * sizeof(int),
					cudaMemcpyHostToDevice));

	std::cout << "cudaMemcpy list to device: " << tm() << " microseconds\n";

	//copy to gpu
	checkCudaErrors(cudaMalloc((void** ) &device_threads, sizeof(dim3)));
	checkCudaErrors(cudaMalloc((void** ) &device_blocks, sizeof(dim3)));

	std::cout << "cudaMalloc device thread data: " << tm()
			<< " microseconds\n";
	checkCudaErrors(
			cudaMemcpy(device_threads, &threadsPerBlock, sizeof(dim3),
					cudaMemcpyHostToDevice));
	checkCudaErrors(
			cudaMemcpy(device_blocks, &blocksPerGrid, sizeof(dim3),
					cudaMemcpyHostToDevice));

	std::cout << "cudaMemcpy thread data to device: " << tm()
			<< " microseconds\n";

	int* A = device_data;
	int* B = device_swp;

	int nThreads = threadsPerBlock.x * threadsPerBlock.y * threadsPerBlock.z
			* blocksPerGrid.x * blocksPerGrid.y * blocksPerGrid.z;

	//slice the array and give it to threads
	for (int width = 2; width < (size << 1); width <<= 1) {
		int slices = size / ((nThreads) * width) + 1;

		std::cout << "mergeSort - width: " << width << ", slices: "
				<< slices << ", nThreads: " << nThreads << '\n';
		tm();


		// call the mergesort kernel
		gpu_mergesort<<<blocksPerGrid, threadsPerBlock>>>(A, B, size, width, slices, device_threads, device_blocks);


		std::cout << "call mergesort kernel: " << tm() << " microseconds\n";

		//swap input/output data
		A = A == device_data ? device_swp : device_data;
		B = B == device_data ? device_swp : device_data;
	}

	//read back from gpu
	tm();
	checkCudaErrors(
			cudaMemcpy(data, A, size * sizeof(int), cudaMemcpyDeviceToHost));

	std::cout << "cudaMemcpy list back to host: " << tm()
				<< " microseconds\n";

	// free memory from gpu
	checkCudaErrors(cudaFree(A));
	checkCudaErrors(cudaFree(B));

	std::cout << "cudaFree: " << tm() << " microseconds\n";
}





int main(int argc, char** argv) {

	std::cout << "Enter the length : ";
	std::cin >> len;
	dim3 threadsPerBlock;
	dim3 blocksPerGrid;

	threadsPerBlock.x = 32;
	threadsPerBlock.y = 32;
	threadsPerBlock.z = 32;

	blocksPerGrid.x = 1;
	blocksPerGrid.y = 1;
	blocksPerGrid.z = 1;
	tm();

	printf(
			"\nthreadsPerBlock :\n x : %d\n y : %d\n z : %d\n\nblocksPerGrid : \n x : %d\n y : %d\n z : %d\n\nTotal threads : %d\n\n",
			threadsPerBlock.x, threadsPerBlock.y, threadsPerBlock.z,
			blocksPerGrid.x, blocksPerGrid.y, blocksPerGrid.z,
			threadsPerBlock.x * threadsPerBlock.y * threadsPerBlock.z
					* blocksPerGrid.x * blocksPerGrid.y * blocksPerGrid.z);



	//Generate random numbers;
	int* data;
	int size = genArray(&data);
	if (!size)
		return -1;


	std::cout << "sorting " << size << " numbers\n\n";

	// merge-sort the data
	mergesort(data, size, threadsPerBlock, blocksPerGrid);

	tm();
	//display(data, size);
}


