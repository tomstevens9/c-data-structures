#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stddef.h>

#include "hashtable.h"
#include "hashtable_dev.h"

/*
 *  TODO: general refactor
 */

/**
 *  Creates a new hash table with the specified size
 */
struct hashtable hashtable_create_sized(int size) {
    struct hashtable hashtable;
    hashtable.entries = 0;
    hashtable.size = size;
    struct hashtable_entry **table = calloc(size, sizeof(size_t));
    hashtable.table = table;
    return hashtable;
}

/**
 *  Creates a new hashtable with the default size
 */
struct hashtable hashtable_create() {
    return hashtable_create_sized(HASHTABLE_START_SIZE);
}


/**
 * Takes a string and returns a pointer to a copy of that string
 */
char *copy_string(const char *str) {
    char *str_copy;

    // generate memory for a copy of str
    str_copy = calloc(strlen(str), sizeof(char));

    // copy str
    strcpy(str_copy, str);

    return str_copy;
}

/**
 * takes a number and returns whether it is prime.
 *
 * assumes n is a natural number
 */
bool is_prime(int n) {
    int i;
    float sqrt_n;
    switch(n) {
        case 1:
            return true;
        case 2:
            return false;
    }
    sqrt_n = sqrt(n);
    for (i = 2; i <= sqrt_n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

/**
 * Takes an integer, n, and returns the smallest prime number equal to or
 * larger than n
 */
int next_prime(int n) {
    if (is_prime(n)) return n;
    while(!is_prime(++n));
    return n;
}

/**
 * Given a key and a value, creates an entry ready to be added into the hashtable
 */
struct hashtable_entry * hashtable_entry_create(const char* key, int value) {
    struct hashtable_entry * entry;
    char* key_copy;

    // allocate memory for entry
    entry = calloc(1, sizeof(struct hashtable_entry));

    // copy key so reference isn't lost
    key_copy = copy_string(key);

    // build entry from keyCopy and value
    entry->key = key_copy;
    entry->value = value;

    return entry;
}

/**
 * Creates a hash from a string
 * 
 *  Sum of ASCII value + position in array
 */
int hashtable_hash(const char* value) {
    int position = 1, result = 0;
    const char *ptr;
    for (ptr = value; *ptr != '\0'; ++ptr, ++position) {
        result += (*ptr * position);
    }
    return result;
}


/**
 * Given a hashtable and a bucket_index, determines whether that bucket is free
 */
int hashtable_is_bucket_free(struct hashtable *hashtable, int bucket_index) {
    struct hashtable_entry *entry = *(hashtable->table + bucket_index);
    return (entry == 0);
}

/**
 * Overwrites an existing hashtable with a new one which includes all the entries
 * from the previous one.
 *
 * The new size is guaranteed to be lowest prime number higher than or equal to
 * double the current table size
 */
void hashtable_resize(struct hashtable *hashtable) {
    int new_table_size, i;
    struct hashtable new_hash_table;
    struct hashtable_entry *entry;

    // create new hash table
    new_table_size = next_prime(hashtable->size * 2);
    new_hash_table = hashtable_create_sized(new_table_size);

    // populate new hash table
    for (i = 0; i < hashtable->size; ++i) {
        // pull entry from old table if not empty
        if (hashtable_is_bucket_free(hashtable, i)) {
            continue;
        }
        entry = hashtable->table[i];

        // enter entry into new table
        hashtable_put(&new_hash_table, entry->key, entry->value);

        // free old entry
        free(entry);
    }

    // overwrite old hashtable with new one
    *hashtable = new_hash_table;
}

/**
 * determines whether the hash table should be resized or not based on it's
 * load factor.
 */
bool hashtable_should_resize(struct hashtable *hashtable) {
    return ((float)hashtable->entries / hashtable->size > 0.75);
}

/**
 *  Put a value into the hashtable
 */
void hashtable_put(struct hashtable *hashtable, const char* key, int value) {
    struct hashtable_entry *entry;
    int hash_value, i, probe_location;

    // resize hash table if reached certain load factor
    if (hashtable_should_resize(hashtable)) {
        hashtable_resize(hashtable);
    }

    // increment how many entries are in the table
    hashtable->entries++;

    // create new entry
    entry = hashtable_entry_create(key, value);

    // calculate hash value from key
    hash_value = hashtable_hash(key);

    // place entry in hash table at ideal_bucket if free
    // otherwise use quadratic probing to find correct location
    for (i = 0; i < hashtable->size; ++i) {
        probe_location = (hash_value + i * i) % hashtable->size;
        if (hashtable_is_bucket_free(hashtable, probe_location) || 
            strcmp(hashtable->table[probe_location]->key, key) == 0) {

            hashtable->table[probe_location] = entry;
            break;
        }
    }
}


/**
 *  Finds the index of an entry in the hashtable
 */
size_t find_entry(struct hashtable *hashtable, const char *key) {
    int hash_value, probe_location, i;

    hash_value = hashtable_hash(key);
    for (i = 0; i < hashtable->size; ++i) {
        probe_location = (hash_value + i * i) % hashtable -> size;

        if (!hashtable_is_bucket_free(hashtable, probe_location) &&
            strcmp(hashtable->table[probe_location]->key, key) == 0) {
            return probe_location;
        }
    }
    return -1;
}


/**
 *  Gets a value from the hashtable
 */
int hashtable_get(struct hashtable *hashtable, const char *key) {
    size_t entry_location = find_entry(hashtable, key);
    return hashtable->table[entry_location]->value;
}

/**
 *  Deletes a value from the hashtable
 */
size_t hashtable_delete(struct hashtable *hashtable, const char *key) {
    size_t entry_location;
    struct hashtable_entry *entry;

    entry_location = find_entry(hashtable, key);
    if (entry_location != -1) {
        entry = hashtable->table[entry_location];
        free(entry);
        hashtable->table[entry_location] = 0;
        --hashtable->entries;
    }
    return entry_location;
}

/**
 *  DEBUG
 *  
 *  prints the hashtable to the console
 */
void hashtable_print(struct hashtable *hashtable) {
    struct hashtable_entry *entry;
    int i;
    printf("\n");
    printf("PRINTING HASH TABLE\n");
    printf("=============================\n");
    printf("= size: %4d, entries: %4d =\n", hashtable->size, hashtable->entries);
    printf("=============================\n");
    for(i = 0; i < hashtable->size; ++i) {
        entry = hashtable->table[i];
        if (entry != 0) {
            printf("%d. {%s, %d}\n", i, hashtable->table[i]->key, hashtable->table[i]->value);
        }
    }
    printf("=============================\n\n");
}
