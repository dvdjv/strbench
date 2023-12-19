#include "str.h"

#include <stdlib.h>
#include <string.h>

#include "const.h"

// same as with linked lists, length is size * NODE_SIZE
str* stralloc(const unsigned size) {
    str* s = malloc(sizeof(str));
    s->size = size;
    s->segments = malloc(sizeof(char*) * size);
    for (int i = 0; i < s->size; i++) s->segments[i] = malloc(NODE_SIZE);
    return s;
}

void strfree(str *s) {
    for (int i = 0; i < s->size; i++) free(s->segments[i]);
    free(s->segments);
    free(s);
}

void strmemset(str* s, int value) {
    for (int i = 0; i < s->size; i++) memset(s->segments[i], value, NODE_SIZE);
}

void strstrcat(str* a, str* b) {
    const unsigned new_size = a->size + b->size;
    a->segments = realloc(a->segments, new_size * sizeof(char*));
    memcpy(a->segments + a->size, b->segments, b->size * sizeof(char*));
    a->size = new_size;
    free(b->segments);
    free(b);
}

void strbench(unsigned size) {
    str *a = stralloc(size);
    str *b = stralloc(size);
    strmemset(a, 'a');
    strmemset(b, 'b');
    strstrcat(a, b);
    strfree(a);
}
