#include "ll.h"

#include <stdlib.h>
#include <string.h>

//for the sake of simplicity str length is always size * NODE_SIZE
llstr* llalloc(unsigned int size) {
    struct llstr_node *head = NULL;
    while (size) {
        struct llstr_node *tail = head;
        head = malloc(sizeof(struct llstr_node));
        head->next = tail;
        --size;
    }
    return head;
}

void llfree(llstr* str) {
    while (str->next) {
        llstr* head = str;
        str = str->next;
        free(head);
    }
}

void llmemset(llstr* str, const int value) {
    do {
        memset(str->payload, value, NODE_SIZE);
        str = str->next;
    } while (str);
}

void llstrcat(llstr *dest, llstr *src) {
    while (dest->next) dest = dest->next;
    dest->next = src;
}

void llbench(unsigned int size) {
    llstr* a = llalloc(size);
    llstr* b = llalloc(size);
    llmemset(a, 'a');
    llmemset(b, 'b');
    llstrcat(a, b);
    llfree(a);
}