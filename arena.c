//
// Created by davja on 20/12/23.
//
#include "arena.h"

#include <assert.h>
#include <stdlib.h>
#include <sys/mman.h>

void arena_initialize(Arena* arena, long size) {
    // Windows equvalent of mmap: VirtualAlloc
    int   protection = PROT_READ   | PROT_WRITE;
    int   flags      = MAP_PRIVATE | MAP_ANONYMOUS;
    char* memory     = mmap(NULL, size, protection, flags, -1, 0);
    assert(memory != MAP_FAILED);

    arena->memory = memory;
    arena->used   = 0;
}

char* arena_allocate_bytes(Arena* arena, long bytes_size) {
    // Mac and Linux will commit the memory on the next page fault.
    // To expicitly commit the memory, use `mprotect` on Mac/Linux and
    // `VirtualAlloc` on Windows.
    char* result  = &arena->memory[arena->used];
    arena->used  += bytes_size;
    return result;
}

#define arena_allocate(arena, type) \
((type*) arena_allocate_bytes((arena), sizeof(type)))

void arena_clear(Arena* arena) {
    arena->used = 0;
}