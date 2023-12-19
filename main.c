#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))

#include "arena.h"
#include "list.h"
#include "ll.h"
#include "str.h"

void benchmark(Arena* arena, const unsigned int size) {
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

    start = clock();
    for (int i = 0; i < BENCHMARK_ITERS; i++) listbench(arena, size);
    end = clock();
    printf("List Clock Start: %lu\n", start);
    printf("List Clock End: %lu\n", end);
    runtime = (double)(end - start) / CLOCKS_PER_SEC / BENCHMARK_ITERS;
    printf("List memory:\t\t\t%0.3f msec/iter\n", runtime * 1000);
}

int main() {
    Arena arena;
    arena_initialize(&arena, 1ll << 36);
  
    benchmark(&arena, 100);
    benchmark(&arena, 1000);
    benchmark(&arena, 5000);
}
