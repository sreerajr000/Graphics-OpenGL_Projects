/*
 * main.cu
 *
 *  Created on: 18-Sep-2017
 *      Author: sree
 */




#include <iostream>
#include <helper_cuda.h>
#include <sys/time.h>
#include "util.h"


using namespace std;

long genArray(long**);


void mergesort(long*, long, dim3, dim3);
__global__ void gpu_mergesort(long*, long*, long, long, long, dim3*, dim3*);
__device__ void gpu_merge(long*, long*, long, long, long);


// profiling
int timing();

#define min(a, b) (a < b ? a : b)


int main(int argc, char** argv) {

    dim3 threadsPerBlock;
    dim3 blocksPerGrid;

    threadsPerBlock.x = 32;  threadsPerBlock.y = 1;   threadsPerBlock.z = 1;
    blocksPerGrid.x = 1;    blocksPerGrid.y = 1;    blocksPerGrid.z = 1;

    cout << "No. of Elements : ";
    cin >> len;
	cout << "\n\n total threads: " << threadsPerBlock.x * threadsPerBlock.y * threadsPerBlock.z *
				 blocksPerGrid.x * blocksPerGrid.y * blocksPerGrid.z  << "\n\n";

    long* data;
    long size = genArray(&data);
	cout << "sorting " << size << " numbers\n\n";

    // merge-sort the data
    mergesort(data, size, threadsPerBlock, blocksPerGrid);

    timing();

    for (int i = 0; i < size; i++) {
        cout << data[i] << '\n';
    }
	//cout << "print list to stdout: " << timing() << " microseconds\n";
}

void mergesort(long* data, long size, dim3 threadsPerBlock, dim3 blocksPerGrid) {

    // Allocate two arrays on the GU
    long* deviceData;
    long* deviceSwap; //for temp storage
    dim3* deviceThreads;
    dim3* deviceBlocks;

    // Actually allocate the two arrays
    timing();
    checkCudaErrors(cudaMalloc((void**) &deviceData, size * sizeof(long)));
    checkCudaErrors(cudaMalloc((void**) &deviceSwap, size * sizeof(long)));
	cout << "cudaMalloc device lists: " << timing() << " microseconds\n";

    // Copy from our input list into the first array
    checkCudaErrors(cudaMemcpy(deviceData, data, size * sizeof(long), cudaMemcpyHostToDevice));
	cout << "cudaMemcpy list to device: " << timing() << " microseconds\n";

    // Copy the thread / block info to the GPU as well

    checkCudaErrors(cudaMalloc((void**) &deviceThreads, sizeof(dim3)));
    checkCudaErrors(cudaMalloc((void**) &deviceBlocks, sizeof(dim3)));

	cout << "cudaMalloc device thread data: " << timing() << " microseconds\n";
    checkCudaErrors(cudaMemcpy(deviceThreads, &threadsPerBlock, sizeof(dim3), cudaMemcpyHostToDevice));
    checkCudaErrors(cudaMemcpy(deviceBlocks, &blocksPerGrid, sizeof(dim3), cudaMemcpyHostToDevice));

	cout << "cudaMemcpy thread data to device: " << timing() << " microseconds\n";

    long* A = deviceData;
    long* B = deviceSwap;

    long nThreads = threadsPerBlock.x * threadsPerBlock.y * threadsPerBlock.z * blocksPerGrid.x * blocksPerGrid.y * blocksPerGrid.z;

    // Slice up the list and give pieces of it to each thread, letting the pieces grow bigger and bigger until the whole list is sorted
    for (int width = 2; width < (size << 1); width <<= 1) {
        long slices = size / ((nThreads) * width) + 1;

		cout << "width: " << width  << " slices: " << slices  << " nThreads: " << nThreads << '\n';
		timing();

        // Actually call the kernel
        gpu_mergesort<<<blocksPerGrid, threadsPerBlock>>>(A, B, size, width, slices, deviceThreads, deviceBlocks);

		cout << "call mergesort kernel: " << timing() << " microseconds\n";

        // Switch the input / output arrays
        A = A == deviceData ? deviceSwap : deviceData;
        B = B == deviceData ? deviceSwap : deviceData;
    }

    // Get the list back from the GPU
    timing();
    checkCudaErrors(cudaMemcpy(data, A, size * sizeof(long), cudaMemcpyDeviceToHost));
	cout << "cudaMemcpy list back to host: " << timing() << " microseconds\n";


    // Free the GPU memory
    checkCudaErrors(cudaFree(A));
    checkCudaErrors(cudaFree(B));
	cout << "cudaFree: " << timing() << " microseconds\n";
}

// Perform mergesort on our section of the data.
__global__ void gpu_mergesort(long* src, long* dst, long size, long width, long slices, dim3* threads, dim3* blocks) {
    unsigned int idx = getIdx(threads, blocks);
    long start = width*idx*slices, middle, end;

    for (long slice = 0; slice < slices; slice++) {
        if (start >= size)
            break;

        middle = min(start + (width >> 1), size);
        end = min(start + width, size);	//for last block
        gpu_merge(src, dst, start, middle, end);
        start += width;
    }
}


// gets called by gpu_mergesort() for each slice
__device__ void gpu_merge(long* src, long* dst, long start, long middle, long end) {
    long i = start;
    long j = middle;
    for (long k = start; k < end; k++) {
        if (i < middle && (j >= end || src[i] < src[j])) {
            dst[k] = src[i];
            i++;
        } else {
            dst[k] = src[j];
            j++;
        }
    }
}
