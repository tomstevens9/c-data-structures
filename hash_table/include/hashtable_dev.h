#ifndef HASHTABLE_DEV_H
#define HASHTABLE_DEV_H
#include <stdbool.h>

// should be prime
#define HASHTABLE_START_SIZE 11  // 1031 is better value to prevent resizing
#define LOAD_FACTOR 0.75

struct hashtable_entry {
    char* key;
    int value;
};

// hashtable
struct hashtable hashtable_create_sized(int);
struct hashtable_entry * hashtable_entry_create(const char*, int);
int hashtable_hash(const char*);
int hashtable_is_bucket_free(struct hashtable*, int);
void hashtable_resize(struct hashtable*);
bool hashtable_should_resize(struct hashtable *hashtable);


// util
char *copy_string(const char*);
bool is_prime(int);
int next_prime(int);

// debug
void hashtable_print(struct hashtable*);

#endif
