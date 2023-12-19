#ifndef STR_H
#define STR_H

typedef struct str {
    unsigned int size;
    char** segments;
} str;

str* stralloc(unsigned int size);

void strfree(str* s);

void strmemset(str* s, int value);

void strstrcat(str* a, str* b);

void strbench(unsigned size);

#endif //STR_H
