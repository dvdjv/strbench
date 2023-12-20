#ifndef LISH_H
#define LIST_H

#include <stddef.h>

#include "arena.h"

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
char* list_push_bytes(List* list, Arena* arena, long bytes_size);

// Free a list by reverting arena `used` to before we started allocating.
// If you have competing lifetimes on the arena, use a slab allocator.

void list_memset(List* list, int value);

void list_strcat(List* a, List* b);

void list_push_str(List *l, Arena *a, const char* str, const size_t len);

void listbench(Arena* arena, long size);

char list_checksum_bench(Arena* arena, const int nwrites);

#endif