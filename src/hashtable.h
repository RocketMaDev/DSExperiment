#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__
#include <stddef.h>
#include "arraylist.h"
#include "linkedlist.h"
#ifndef NODE_TYPE
#define NODE_TYPE int
#endif
typedef struct {
    NODE_TYPE *arr;
    unsigned size;
    unsigned count;
    unsigned long long used;
} LinearHashTable;

typedef struct {
    LinkedList *buckets;
    unsigned size;
    unsigned count;
} LinkedHashTable;

typedef size_t (*hashfunc_t)(NODE_TYPE v);

int LinearHashTableInit(LinearHashTable *table, unsigned size);
int LinearHashTableInsert(LinearHashTable *table, NODE_TYPE v, hashfunc_t hasher);
int LinearHashTableFind(const LinearHashTable *table, NODE_TYPE v, hashfunc_t hasher, Nullable unsigned *cmpTimes);
void LinearHashTableRelease(LinearHashTable *table);

int LinkedHashTableInit(LinkedHashTable *table, unsigned size);
int LinkedHashTableInsert(LinkedHashTable *table, NODE_TYPE v, hashfunc_t hasher);
int LinkedHashTableFind(const LinkedHashTable *table, NODE_TYPE v, hashfunc_t hasher, Nullable unsigned *cmpTimes);
void LinkedHashTableRelease(LinkedHashTable *table);

#define HashTableInit(table, size) _Generic((table), \
        LinkedHashTable *: LinkedHashTableInit, \
        LinearHashTable *: LinearHashTableInit)(table, size);
#define HashTableInsert(table, v, hasher) _Generic((table), \
        LinkedHashTable *: LinkedHashTableInsert, \
        LinearHashTable *: LinearHashTableInsert)(table, v, hasher);
#define HashTableFind(table, v, hasher, cmpTimes) _Generic((table), \
        LinkedHashTable *: LinkedHashTableFind, \
        LinearHashTable *: LinearHashTableFind)(table, v, hasher, cmpTimes);
#define HashTableRelease(table) _Generic((table), \
        LinkedHashTable *: LinkedHashTableRelease, \
        LinearHashTable *: LinearHashTableRelease)(table);
#endif
