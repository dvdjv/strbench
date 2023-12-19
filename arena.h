typedef struct {
  char* memory;
  long  used;
} Arena;

// Reserve a huge amount of the address space.
// Since this program also uses malloc, we can't reserve
// the whole address space, so the reccomended value is 64 GB.
static void arena_initialize(Arena* arena, long size) {
  // Windows equvalent of mmap: VirtualAlloc
  int   protection = PROT_READ   | PROT_WRITE;
  int   flags      = MAP_PRIVATE | MAP_ANONYMOUS;
  char* memory     = mmap(NULL, size, protection, flags, -1, 0);
  assert(memory != MAP_FAILED);

  arena->memory = memory;
  arena->used   = 0;
}

static char* arena_allocate_bytes(Arena* arena, long bytes_size) {
  // Mac and Linux will commit the memory on the next page fault.
  // To expicitly commit the memory, use `mprotect` on Mac/Linux and
  // `VirtualAlloc` on Windows.
  char* result  = &arena->memory[arena->used];
  arena->used  += bytes_size;
  return result;
}

#define arena_allocate(arena, type) \
  ((type*) arena_allocate_bytes((arena), sizeof(type)))

static void arena_clear(Arena* arena) {
  arena->used = 0;
}
