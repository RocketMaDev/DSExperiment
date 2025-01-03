#include "hashtable.h"
#include "linkedlist.h"
#include "rerror.h"
#include <stdlib.h>
/* Linear */
int LinearHashTableInit(LinearHashTable *table, unsigned size) {
    table->size = 0;
    if (!size)
        return -RERR_EMPTY;
    if (size > 63)
        return -RERR_OVERFLOW;
    table->arr = malloc(size * sizeof(NODE_TYPE));
    if (!table->arr)
        return -RERR_OOM;
    table->count = 0;
    table->size = size;
    table->used = 0;
    return -RERR_OK;
}

int LinearHashTableInsert(LinearHashTable *table, NODE_TYPE v, hashfunc_t hasher) {
    if (!table->size)
        return -RERR_EMPTY;
    if (table->size == table->count)
        return -RERR_OVERFLOW;
    register unsigned long hash = hasher(v) % table->size;
    unsigned offset;
    if (LinearHashTableFind(table, v, hasher, &offset) != -RERR_NOTFOUND)
        return -RERR_EXISTED;
    hash += offset;
    table->arr[hash] = v;
    table->used |= (1 << hash);
    table->count++;
    return -RERR_OK;
}

int LinearHashTableFind(const LinearHashTable *table, NODE_TYPE v, hashfunc_t hasher, Nullable unsigned *cmpTimes) {
    if (!table->size)
        return -RERR_EMPTY;
    register unsigned long hash = hasher(v) % table->size;
    unsigned cmp_times = 0;
    while (table->used & (1 << hash)) {
        cmp_times++;
        if (table->arr[hash] == v) {
            if (cmpTimes)
                *cmpTimes = cmp_times;
            return hash;
        }
        hash = (hash + 1) % table->size;
    }
    if (cmpTimes)
        *cmpTimes = cmp_times;
    return -RERR_NOTFOUND;
}

void LinearHashTableRelease(LinearHashTable *table) {
    table->size = 0;
    table->count = 0;
    free(table->arr);
    table->arr = NULL;
}

/* Linked */
int LinkedHashTableInit(LinkedHashTable *table, unsigned size) {
    if (!size)
        return -RERR_EMPTY;
    table->size = 0;
    table->count = 0;
    table->buckets = malloc(size * sizeof(LinkedList));
    if (!table->buckets)
        return -RERR_OOM;
    for (unsigned i = 0; i < size; i++)
        LinkedListInit(table->buckets + i);
    table->size = size;
    return -RERR_OK;
}

int LinkedHashTableInsert(LinkedHashTable *table, NODE_TYPE v, hashfunc_t hasher) {
    if (!table->size)
        return -RERR_EMPTY;
    register unsigned hash = hasher(v) % table->size;
    if (LinkedListFind(table->buckets + hash, v) != -RERR_NOTFOUND)
        return -RERR_EXISTED;
    int err;
    err = LinkedListPrepend(table->buckets + hash, v);
    if (err)
        return err;
    table->count++;
    return -RERR_OK;
}

int LinkedHashTableFind(const LinkedHashTable *table, NODE_TYPE v, hashfunc_t hasher, Nullable unsigned *cmpTimes) {
    if (!table->size)
        return -RERR_EMPTY;
    register unsigned hash = hasher(v) % table->size;
    int err = LinkedListFind(table->buckets + hash, v);
    if (cmpTimes) {
        if (err == -RERR_NOTFOUND)
            *cmpTimes = LinkedListSize(table->buckets + hash) + 1;
        else
            *cmpTimes = err + 1; // idx in bucket
    }
    if (err < 0)
        return err;
    else
        return hash;
}

void LinkedHashTableRelease(LinkedHashTable *table) {
    if (!table->size)
        return;
    for (unsigned i = 0; i < table->size; i++)
        LinkedListRelease(table->buckets + i);
    free(table->buckets);
    table->buckets = NULL;
    table->count = 0;
    table->size = 0;
}
