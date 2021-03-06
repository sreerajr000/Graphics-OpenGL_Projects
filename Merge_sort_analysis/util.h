/*
 * util.h
 *
 *  Created on: 18-Sep-2017
 *      Author: sree
 */

#ifndef UTIL_H_
#define UTIL_H_
int len;


// GPU helper function
// calculate the id of the current thread
__device__ unsigned int getIdx(dim3* threads, dim3* blocks) {
    int x;
    return threadIdx.x + threadIdx.y * (x  = threads->x) + threadIdx.z * (x *= threads->y) + blockIdx.x  * (x *= threads->z) + blockIdx.y  * (x *= blocks->z) +
           blockIdx.z  * (x *= blocks->y);
}



timeval tStart;
int timing() {
    timeval tEnd;
    gettimeofday(&tEnd, 0);
    int t = (tEnd.tv_sec - tStart.tv_sec) * 1000000 + tEnd.tv_usec - tStart.tv_usec;
    tStart = tEnd;
    return t;
}


// read data into a minimal linked list
typedef struct {
    int v;
    void* next;
} LinkNode;


long genArray(long** list) {
    timing();
    long v, size = 0;
    LinkNode* node = 0;
    LinkNode* first = 0;
    while (len--) {
    	v = rand() % INT_MAX;
        LinkNode* next = new LinkNode();
        next->v = v;
        if (node)
            node->next = next;
        else
            first = next;
        node = next;
        size++;
    }


    if (size) {
        *list = new long[size];
        LinkNode* node = first;
        long i = 0;
        while (node) {
            (*list)[i++] = node->v;
            node = (LinkNode*) node->next;
        }

    }

    return size;
}

#endif /* UTIL_H_ */
