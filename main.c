#include <stdio.h>
#include <time.h>

#include "ll.h"
#include "str.h"


void benchmark(const unsigned int size) {
    clock_t start, end;
    double runtime;
    printf("\n\n=== Benchmarking linked list vs contiguous memory, size: %u ===\n\n", size);

    start = clock();
    for (int i = 0; i < BENCHMARK_ITERS; i++) llbench(size);
    end = clock();
    runtime = (double)(end - start) / CLOCKS_PER_SEC / BENCHMARK_ITERS;
    printf("Linked List:\t\t\t%0.3f msec/iter\n", runtime * 1000);

    start = clock();
    for (int i = 0; i < BENCHMARK_ITERS; i++) strbench(size);
    end = clock();
    runtime = (double)(end - start) / CLOCKS_PER_SEC / BENCHMARK_ITERS;
    printf("Contiguous memory:\t\t%0.3f msec/iter\n", runtime * 1000);
}

int main() {
    benchmark(100);
    benchmark(1000);
    benchmark(5000);
}
