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
