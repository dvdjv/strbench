//
// Created by David on 19/12/2023.
//

#include "arr.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "const.h"

#define PAGE_SIZE 4096
#define ALLOC_SIZE (1ll << 32)

arr arr_alloc(Arena *arena) {
    arr array = { .len = 0, .capacity = ALLOC_SIZE };
    char* region = arena_allocate_bytes(arena, ALLOC_SIZE);
    array.mem = region;
    return array;
}

void arr_push(arr* array, const char* str, const size_t len) {
    const size_t new_len = array->len + len;
    memcpy(array->mem + array->len, str, len);
    array->len = new_len;
}

char arr_checksum(const arr* array) {
    char checksum = 0;
    for (size_t i = 0; i < array->len; i++) checksum ^= array->mem[i];
    return checksum;
}

char arr_benchmark(Arena *arena, const int nwrites) {
    arr buffers[4] = {
        arr_alloc(arena),
        arr_alloc(arena),
        arr_alloc(arena),
        arr_alloc(arena)
    };

    char checksums[4] = {0, 0, 0, 0};

    for (int i = 0; i < nwrites; i++) {
        const unsigned long index = rand() % (sizeof(buffers) / sizeof(arr));
        arr* buffer = &buffers[index];
        const char* line = phrases[(rand() % (sizeof(phrases) / sizeof(void*)))];
        const size_t linelen = strlen(line);
        for (int j = 0; j < linelen; j++) checksums[index] ^= line[j];
        arr_push(buffer, line, linelen);
    }

    const char checksum = arr_checksum(&buffers[0]) ^
                          arr_checksum(&buffers[1]) ^
                          arr_checksum(&buffers[2]) ^
                          arr_checksum(&buffers[3]);
    const char verity = checksums[0] ^ checksums[1] ^
                        checksums[2] ^ checksums[3];
    assert(checksum == verity);

    arena_clear(arena);

    return checksum;
}
