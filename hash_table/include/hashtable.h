#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <stddef.h>

struct hashtable {
    int size;
    int entries;
    struct hashtable_entry **table;
};

struct hashtable hashtable_create();
int hashtable_get(struct hashtable*, const char*);
void hashtable_put(struct hashtable*, const char*, int);
size_t hashtable_delete(struct hashtable*, const char*);

#endif
