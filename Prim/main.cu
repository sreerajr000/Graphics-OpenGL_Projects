/*
 * main.cu
 *
 *  Created on: 26-Sep-2017
 *      Author: sree
 */

#include <iostream>
#include <cstdio>
#include <helper_cuda.h>

// Number of vertices in the graph
#define V 10

// calculate the id of the current thread
__device__ unsigned int getIdx(dim3* threads, dim3* blocks) {
    int x;
    return threadIdx.x + threadIdx.y * (x  = threads->x) + threadIdx.z * (x *= threads->y) + blockIdx.x  * (x *= threads->z) + blockIdx.y  * (x *= blocks->z) +
           blockIdx.z  * (x *= blocks->y);
}


__global__ void gpuPrim(int *device_graph, int vertices, long nThreads, dim3* threads, dim3* blocks){
	unsigned int idx = getIdx(threads, blocks);
	int start = idx * (vertices/nThreads);
	int end = min(start + (int)(vertices/nThreads), vertices);

	__shared__ int key[V];
	__shared__ bool mstSet[V];
	__shared__ int parent[V];

	for(int i = start; i < end; i++)
		key[i] = INT_MAX, mstSet[i] = false;

	key[0] = 0;
	parent[0] = -1;

/*	// The MST will have V vertices
	for (int count = 0; count < V - 1; count++) {
		// Pick the minimum key vertex from the set of vertices
		// not yet included in MST
		int minimum = INT_MAX, u;

		for (int v = 0; v < V; v++)
			if (mstSet[v] == false && key[v] < minimum)
				minimum = key[v], u = v;


		// Add the picked vertex to the MST Set
		mstSet[u] = true;

		// Update key value and parent index of the adjacent vertices of
		// the picked vertex. Consider only those vertices which are not yet
		// included in MST
		for (int v = 0; v < V; v++)

			// graph[u][v] is non zero only for adjacent vertices of m
			// mstSet[v] is false for vertices not yet included in MST
			// Update the key only if graph[u][v] is smaller than key[v]
			if (graph[u*V+v] && mstSet[v] == false && graph[u*V+v] < key[v])
				parent[v] = u, key[v] = graph[u*V+v];
	}*/


}


int minKey(int key[], bool mstSet[]) {
	int minimum = INT_MAX, min_index;

	for (int v = 0; v < V; v++)
		if (mstSet[v] == false && key[v] < minimum)
			minimum = key[v], min_index = v;

	return min_index;
}

int printMST(int parent[], int n, int graph[V*V]) {
	printf("Edge   Weight\n");
	for (int i = 1; i < V; i++)
		printf("%d - %d    %d \n", parent[i], i, graph[i*V + parent[i]]);
}

// Function to construct and print MST for a graph represented using adjacency
// matrix representation
void primMSTSerial(int graph[V*V]) {
	int parent[V]; // Array to store constructed MST
	int key[V];   // Key values used to pick minimum weight edge in cut
	bool mstSet[V];  // To represent set of vertices not yet included in MST

	// Initialize all keys as INFINITE
	for (int i = 0; i < V; i++)
		key[i] = INT_MAX, mstSet[i] = false;

	// Always include first 1st vertex in MST.
	key[0] = 0;     // Make key 0 so that this vertex is picked as first vertex
	parent[0] = -1; // First node is always root of MST

	// The MST will have V vertices
	for (int count = 0; count < V - 1; count++) {
		// Pick the minimum key vertex from the set of vertices
		// not yet included in MST
		int minimum = INT_MAX, u;

		for (int v = 0; v < V; v++)
			if (mstSet[v] == false && key[v] < minimum)
				minimum = key[v], u = v;


		// Add the picked vertex to the MST Set
		mstSet[u] = true;

		// Update key value and parent index of the adjacent vertices of
		// the picked vertex. Consider only those vertices which are not yet
		// included in MST
		for (int v = 0; v < V; v++)

			// graph[u][v] is non zero only for adjacent vertices of m
			// mstSet[v] is false for vertices not yet included in MST
			// Update the key only if graph[u][v] is smaller than key[v]
			if (graph[u*V+v] && mstSet[v] == false && graph[u*V+v] < key[v])
				parent[v] = u, key[v] = graph[u*V+v];
	}

	printMST(parent, V, graph);
}

void primMSTParallel(int graph[V*V], dim3 threadsPerBlock, dim3 blocksPerGrid){
	int *device_graph;

	dim3* deviceThreads;
	dim3* deviceBlocks;

	checkCudaErrors(cudaMalloc((void**) &device_graph, V * V * sizeof(int)));
	checkCudaErrors(cudaMemcpy(device_graph, graph, V * V * sizeof(int), cudaMemcpyHostToDevice));

	checkCudaErrors(cudaMalloc((void**) &deviceThreads, sizeof(dim3)));
	checkCudaErrors(cudaMalloc((void**) &deviceBlocks, sizeof(dim3)));

	checkCudaErrors(cudaMemcpy(deviceThreads, &threadsPerBlock, sizeof(dim3), cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMemcpy(deviceBlocks, &blocksPerGrid, sizeof(dim3), cudaMemcpyHostToDevice));

	long nThreads = threadsPerBlock.x * threadsPerBlock.y * threadsPerBlock.z * blocksPerGrid.x * blocksPerGrid.y * blocksPerGrid.z;
	std::cout << "nThreads : " << nThreads << std::endl;

	gpuPrim<<<blocksPerGrid, threadsPerBlock>>>(device_graph, V, nThreads, deviceThreads, deviceBlocks);
}

int main() {
	double start, stop;

	int graph[V*V];
	srand((unsigned) time(NULL));

	for (int i = 0; i < V; ++i) {
		for (int j = i; j < V; ++j) {
			if (i == j)
				graph[i*V+j] = 0;
			else
				graph[i*V+j] = graph[j*V+i] = rand() % 10;
		}
	}


	dim3 threadsPerBlock;
	dim3 blocksPerGrid;

	threadsPerBlock.x = 32;  threadsPerBlock.y = 1;   threadsPerBlock.z = 1;
	blocksPerGrid.x = 1;    blocksPerGrid.y = 1;    blocksPerGrid.z = 1;

	std::cout << "Total threads: " << threadsPerBlock.x * threadsPerBlock.y * threadsPerBlock.z *
				 blocksPerGrid.x * blocksPerGrid.y * blocksPerGrid.z  << "\n\n";

	primMSTSerial(graph);

	primMSTParallel(graph, threadsPerBlock, blocksPerGrid);

	return 0;
}