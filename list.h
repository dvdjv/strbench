#include "const.h"

typedef struct ListNode ListNode;
typedef struct List     List;

#define MEMORY_SIZE (8 * 1024 * 1024 - 8)

struct ListNode {
  long      used;
  ListNode* next;
  char      memory[MEMORY_SIZE];
};

// `head` and `tail` may be null.
struct List {
  ListNode* head;
  ListNode* tail;
};

// bytes_size must be less than or equal to MEMORY_SIZE
static char* list_push_bytes(List* list, Arena* arena, long bytes_size) {
  if (list->head == NULL) {
    ListNode* new_head = arena_allocate(arena, ListNode);
    new_head->used     = 0;
    new_head->next     = NULL;
    list->head         = new_head;
    list->tail         = new_head;
  }

  ListNode* tail = list->tail;
  if (bytes_size > sizeof tail->memory - tail->used) {
    ListNode* new_tail = arena_allocate(arena, ListNode);
    new_tail->used     = bytes_size;
    new_tail->next     = NULL;
    list->tail->next   = new_tail;
    list->tail         = new_tail;
    return new_tail->memory;
  }

  char* result  = &tail->memory[tail->used];
  tail->used   += bytes_size;
  return result;
}

// Free a list by reverting arena `used` to before we started allocating.
// If you have competing lifetimes on the arena, use a slab allocator.

static void list_memset(List* list, int value) {
  for (ListNode* i = list->head; i != NULL; i = i->next) {
    memset(i->memory, value, i->used);
  }
}

static void list_strcat(List* a, List* b) {
  a->tail->next = b->head;
  a->tail       = b->tail;
}

static void list_push_str(List *l, Arena *a, const char* str, const size_t len) {
  memcpy(list_push_bytes(l, a, len), str, len);
}

static char list_chechsum(const List* l) {
  char checksum = 0;
  const ListNode *head = l->head;
  while (head) {
    for (long i = 0; i < head->used; i++) checksum ^= head->memory[i];
    head = head->next;
  }
  return checksum;
}

static void listbench(Arena* arena, long size) {
  List a = {};
  List b = {};
  while (size > 0) {
    list_push_bytes(&a, arena, min(size, MEMORY_SIZE));
    list_push_bytes(&b, arena, min(size, MEMORY_SIZE));
    size -= MEMORY_SIZE;
  }

  list_memset(&a, 'a');
  list_memset(&b, 'b');
  list_strcat(&a, &b);
  arena_clear(arena);
}

static char list_checksum_bench(Arena* arena, const int nwrites) {
  List buffers[4] = { {}, {}, {}, {} };
  char checksums[4] = {0, 0, 0, 0};

  for (int i = 0; i < nwrites; i++) {
    const int index = rand() % (sizeof(buffers) / sizeof(List));
    List *buffer = &buffers[index];
    const char* line = phrases[(rand() % (sizeof(phrases) / sizeof(void*)))];
    const size_t linelen = strlen(line);
    for (int j = 0; j < linelen; j++) checksums[index] ^= line[j];
    list_push_str(buffer, arena, line, linelen);
  }

  const char checksum = list_chechsum(&buffers[0]) ^ list_chechsum(&buffers[1]) ^
                        list_chechsum(&buffers[2]) ^ list_chechsum(&buffers[3]);
  const char verity = checksums[0] ^ checksums[1] ^ checksums[2] ^ checksums[3];
  assert(checksum == verity);

  arena_clear(arena);
  return checksum;
}