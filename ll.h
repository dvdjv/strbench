#ifndef LL_H
#define LL_H

#include "const.h"

typedef struct llstr_node {
    struct llstr_node *next;
    char payload[NODE_SIZE];
} llstr;

llstr* llalloc(unsigned int size);

void llfree(llstr* str);

void llmemset(llstr* str, int value);

void llstrcat(llstr *dest, llstr *src);

void llbench(unsigned int size);

#endif //LL_H
