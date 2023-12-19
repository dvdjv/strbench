//
// Created by David on 19/12/2023.
//

#ifndef ARR_H
#define ARR_H
#include <stddef.h>

#include "arena.h"

typedef struct arr {
    char *mem;
    size_t len, capacity;
} arr;

arr arr_alloc();

// void arr_free(const arr *array);

void arr_push(arr *array, const char* str, size_t len);

char arr_checksum(const arr *array);

char arr_benchmark(Arena *arena, int nwrites);

#endif //ARR_H
