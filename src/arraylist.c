#include "arraylist.h"
#include <stdlib.h>
#include <string.h>
#define MIN_SIZE 16

#define IFOOM(ptr) \
    if (__builtin_expect(!ptr, 0))  \
        return -RERR_OOM

#define IFERR(stat) \
    if ((err = stat)) \
        return err

static int grow(ArrayList *list, int size) {
    if (size <= 0) {
        // auto expand
        if (list->capacity >= MAX_SIZE)
            return -RERR_OVERFLOW;
        size = list->capacity + (list->capacity >> 1);
        if (size > MAX_SIZE)
            size = MAX_SIZE;
        else if (size < MIN_SIZE)
            size = MIN_SIZE;
    } else 
        size = size <= 11 ? MIN_SIZE : size;
    NODE_TYPE *array = (NODE_TYPE *)realloc(list->arr, sizeof(NODE_TYPE) * size);
    IFOOM(array);
    list->arr = array;
    list->capacity = size;
    return -RERR_OK;
}

int ArrayListInit(ArrayList *list, unsigned int initSize) {
    if (initSize > MAX_SIZE)
        return -RERR_OVERFLOW;
    list->arr = NULL;
    list->size = 0;
    list->capacity = 0;
    unsigned int size = initSize < MIN_SIZE ? MIN_SIZE : initSize;
    int err;
    IFERR(grow(list, size));
    return -RERR_OK;
}

void ArrayListClear(ArrayList *list) {
    list->size = 0;
}

void ArrayListRelease(ArrayList *list) {
    list->capacity = 0;
    list->size = 0;
    free(list->arr);
    list->arr = NULL;
}

int ArrayListAppend(ArrayList *list, NODE_TYPE v) {
    if (list->capacity <= list->size) {
        int err;
        IFERR(grow(list, -1));
    }
    list->arr[list->size++] = v;
    return -RERR_OK;
}

int ArrayListInsert(ArrayList *list, unsigned int index, NODE_TYPE v) {
    if (index == list->size)
        return ArrayListAppend(list, v);
    else if (index > list->size)
        return -RERR_OVERFLOW;
    if (list->capacity <= list->size) {
        int err;
        IFERR(grow(list, -1));
    }
    memmove(list->arr + index + 1, list->arr + index, (list->size - index) * sizeof(NODE_TYPE));
    list->arr[index] = v;
    list->size++;
    return -RERR_OK;
}

int ArrayListPrepend(ArrayList *list, NODE_TYPE v) {
    return ArrayListInsert(list, 0, v);
}

int ArrayListDelete(ArrayList *list, unsigned int index, Nullable NODE_TYPE *recv) {
    if (index >= list->size)
        return -RERR_OVERFLOW;
    if (recv)
        *recv = list->arr[index];
    if (index < --list->size) // optimize for stack.pop
        memmove(list->arr + index, list->arr + index + 1, (list->size - index) * sizeof(NODE_TYPE));
    return -RERR_OK;
}

int ArrayListSet(ArrayList *list, unsigned int index, NODE_TYPE v) {
    if (index >= list->size)
        return -RERR_OVERFLOW;
    list->arr[index] = v;
    return -RERR_OK;
}

int ArrayListGet(const ArrayList *list, unsigned int index, Nullable NODE_TYPE *recv) {
    if (index >= list->size)
        return -RERR_OVERFLOW;
    if (recv)
        *recv = list->arr[index];
    return -RERR_OK;
}

unsigned int ArrayListSize(const ArrayList *list) {
    return list->size;
}

int ArrayListFind(const ArrayList *list, NODE_TYPE v) {
    for (unsigned int i = 0; i < list->size; i++) {
#ifdef OVERRIDE_CMP
        if (NodeCmp(list->arr[i], v)) 
#else
        if (list->arr[i] == v)
#endif
            return i;
    }
    return -RERR_NOTFOUND;
}

int ArrayListRemove(ArrayList *list, NODE_TYPE v) {
    int idx;
    int count = 0;
    while ((idx = ArrayListFind(list, v)) != -RERR_NOTFOUND) {
        ArrayListDelete(list, (unsigned int)idx, NULL);
        count++;
    }
    return count ? count : -RERR_NOTFOUND;
}

void ArrayListTraverse(ArrayList *list, void *buf, void (*func)(unsigned int index, NODE_TYPE v, void *buf)) {
    for (unsigned int i = 0; i < list->size; i++)
        func(i, list->arr[i], buf);
}

int ArrayListExtend(ArrayList *dst, const ArrayList *src) {
    unsigned int size = dst->size + src->size;
    int err;
    if (size > MAX_SIZE)
        return -RERR_OVERFLOW;
    if (size > dst->capacity)
        IFERR(grow(dst, size));
    memcpy(dst->arr + dst->size, src->arr, src->size * sizeof(NODE_TYPE));
    dst->size = size;
    return -RERR_OK;
}
#undef IFOOM
#undef IFERR
