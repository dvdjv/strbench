#ifndef ARENA_H
#define ARENA_H

typedef struct {
  char* memory;
  long  used;
} Arena;

// Reserve a huge amount of the address space.
// Since this program also uses malloc, we can't reserve
// the whole address space, so the reccomended value is 64 GB.
void arena_initialize(Arena* arena, long size);

char* arena_allocate_bytes(Arena* arena, long bytes_size);

#define arena_allocate(arena, type) \
  ((type*) arena_allocate_bytes((arena), sizeof(type)))

void arena_clear(Arena* arena);

#endif
