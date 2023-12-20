#include <stdio.h>
#include <time.h>
#include <sys/mman.h>

#include <stdlib.h>

#include "arena.h"
#include "arr.h"
#include "list.h"
#include "ll.h"
#include "str.h"

void benchmark(Arena* arena, const unsigned int size) {
    clock_t start, end;
    double runtime;
    printf("\n\n=== Benchmarking linked list vs contiguous memory, size: %u ===\n\n", size);

    srand(1);
    start = clock();
    char checksum = 0;
    for (int i = 0; i < BENCHMARK_ITERS; i++) checksum ^= arr_benchmark(arena, size);
    end = clock();
    runtime = (double)(end - start) / CLOCKS_PER_SEC / BENCHMARK_ITERS;
    printf("Contiguous memory:\t\t%0.3f msec/iter\t\tchecksum = %d\n", runtime * 1000, checksum);

    srand(1);
    start = clock();
    checksum = 0;
    for (int i = 0; i < BENCHMARK_ITERS; i++) checksum ^= list_checksum_bench(arena, size);
    end = clock();
    runtime = (double)(end - start) / CLOCKS_PER_SEC / BENCHMARK_ITERS;
    printf("List memory:\t\t\t%0.3f msec/iter\t\tchecksum = %d\n", runtime * 1000, checksum);
}

int main() {
    Arena arena;
    arena_initialize(&arena, 1ll << 34);
  
    benchmark(&arena, 1000);
    benchmark(&arena, 1000000);
    benchmark(&arena, 10000000);
    benchmark(&arena, 20000000);
}
