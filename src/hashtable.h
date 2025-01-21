#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__
#include <stddef.h>
#include "arraylist.h"
#include "linkedlist.h"
#ifndef NODE_TYPE
#define NODE_TYPE int
#endif

typedef size_t (*hashfunc_t)(NODE_TYPE v);

typedef struct {
    NODE_TYPE *arr;
    unsigned size;
    unsigned count;
    unsigned long long used;
    hashfunc_t hasher;
} LinearHashTable;

typedef struct {
    LinkedList *buckets;
    unsigned size;
    unsigned count;
    hashfunc_t hasher;
} LinkedHashTable;

/**
 * Initialize a linear hash table.
 *
 * @param table     An uninitialized table.
 * @param size      The capacity of table.
 * @param hasher    The function to hash `v`. The result will be `%`ed by `size`.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_OOM      Insufficient memory when running.
 * @exception RERR_OVERFLOW `size > 64`, bitmap is not enough.
 * @exception RERR_EMPTY    `size == 0`.
 */
int LinearHashTableInit(LinearHashTable *table, unsigned size, hashfunc_t hasher);
/**
 * Insert an element into hash table, conflict will be solved linearly.
 *
 * @param table     A hash table.
 * @param v         The element to insert.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_EMPTY    The table is uninitialized.
 * @exception RERR_OVERFLOW The table is full.
 * @exception RERR_EXISTED  The element is existed in table.
 */
int LinearHashTableInsert(LinearHashTable *table, NODE_TYPE v);
/**
 * Find an element in hash table, return the index of it. As this is just
 * a poc to search, so I didn't implement the way to write the element.
 *
 * @param table     A hash table.
 * @param v         The element to search.
 * @param cmpTimes  If not `NULL`, store the comparison times.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_EMPTY    The table is uninitialized.
 * @exception RERR_NOTFOUND The element is not found in table.
 */
int LinearHashTableFind(const LinearHashTable *table, NODE_TYPE v, Nullable unsigned *cmpTimes);
/**
 * Release the table. Destructor.
 *
 * @param table A hash table.
 */
void LinearHashTableRelease(LinearHashTable *table);

/**
 * Initialize a linked hash table.
 *
 * @param table     An uninitialized table.
 * @param size      The amount of buckets in table.
 * @param hasher    The function to hash `v`. The result will be `%`ed by `size`.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_OOM      Insufficient memory when running.
 * @exception RERR_EMPTY    `size == 0`.
 */
int LinkedHashTableInit(LinkedHashTable *table, unsigned size, hashfunc_t hasher);
/**
 * Insert an element into hash table, conflict will be solved linearly.
 *
 * @param table     A hash table.
 * @param v         The element to insert.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_EMPTY    The table is uninitialized.
 * @exception RERR_OOM      Insufficient memory to insert the element.
 * @exception RERR_EXISTED  The element is existed in table.
 */
int LinkedHashTableInsert(LinkedHashTable *table, NODE_TYPE v);
/**
 * Find an element in hash table, return the index of it. As this is just
 * a poc to search, so I didn't implement the way to write the element.
 *
 * @param table     A hash table.
 * @param v         The element to search.
 * @param cmpTimes  If not `NULL`, store the comparison times.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_EMPTY    The table is uninitialized.
 * @exception RERR_NOTFOUND The element is not found in table.
 */
int LinkedHashTableFind(const LinkedHashTable *table, NODE_TYPE v, Nullable unsigned *cmpTimes);
/**
 * Release the table. Destructor.
 *
 * @param table A hash table.
 */
void LinkedHashTableRelease(LinkedHashTable *table);

#define HashTableInit(table, size, hasher) _Generic((table), \
        LinkedHashTable *: LinkedHashTableInit, \
        LinearHashTable *: LinearHashTableInit)(table, size, hasher);
#define HashTableInsert(table, v) _Generic((table), \
        LinkedHashTable *: LinkedHashTableInsert, \
        LinearHashTable *: LinearHashTableInsert)(table, v);
#define HashTableFind(table, v, cmpTimes) _Generic((table), \
        LinkedHashTable *: LinkedHashTableFind, \
        LinearHashTable *: LinearHashTableFind)(table, v, cmpTimes);
#define HashTableRelease(table) _Generic((table), \
        LinkedHashTable *: LinkedHashTableRelease, \
        LinearHashTable *: LinearHashTableRelease)(table);
#endif
