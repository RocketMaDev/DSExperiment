#include "arrayoperation.h"
#include "arraylist.h"
#include "rerror.h"
#include <stdlib.h>

#define IFERR(stat) \
    if ((err = stat)) \
        return err

int Union(ArrayList *a, const ArrayList *b, Nullable ArrayList *ans) {
    if (!a->size)
        return ArrayListExtend(ans ? ans : a, b);
    if (!b->size) {
        if (ans)
            return ArrayListExtend(ans, b);
        return -RERR_OK;
    }
    int err;
    ArrayList *target = ans ? ans : a;
    if (ans)
        IFERR(ArrayListExtend(ans, a));
    for (unsigned int i = 0; i < b->size; i++)
        if (ArrayListFind(target, b->arr[i]) == -RERR_NOTFOUND)
            IFERR(ArrayListAppend(target, b->arr[i]));
    return -RERR_OK;
}

int Intersection(ArrayList *a, const ArrayList *b, Nullable ArrayList *ans) {
    if (!a->size)
        return -RERR_OK;
    if (!b->size) {
        ArrayListClear(ans ? ans : a);
        return -RERR_OK;
    }
    ArrayList buf;
    int err;
    IFERR(ArrayListInit(&buf, a->size)); // a->size as it will not be larger than a
    for (unsigned int i = 0; i < a->size; i++)
        if (ArrayListFind(b, a->arr[i]) != -RERR_NOTFOUND)
            ArrayListAppend(&buf, a->arr[i]); // ignore the result as always succeed
    ArrayList *target = ans ? ans : a;
    free(target->arr);
    target->arr = buf.arr;
    target->size = buf.size;
    target->capacity = buf.capacity;
    return -RERR_OK;
}

int Difference(ArrayList *a, const ArrayList *b, Nullable ArrayList *ans) {
    if (!a->size)
        return -RERR_OK;
    if (!b->size) {
        if (ans)
            return ArrayListExtend(ans, a);
        return -RERR_OK;
    }
    int err;
    ArrayList *target = ans ? ans : a;
    if (ans)
        IFERR(ArrayListExtend(ans, a));
    for (unsigned int i = 0; i < b->size; i++)
        ArrayListRemove(target, b->arr[i]);
    return -RERR_OK;
}

__attribute__((always_inline))
static inline int merge_in_place(ArrayList *a, const ArrayList *b) {
    if (!a->size)
        return ArrayListExtend(a, b);
    if (!b->size)
        return -RERR_OK;
    int err;
    unsigned int ai = 0, bi = 0;
    while (ai < a->size && bi < b->size) {
        if (a->arr[ai] > b->arr[bi])
            IFERR(ArrayListInsert(a, ai, b->arr[bi++]));
        ai++;
    }
    while (bi < b->size)
        IFERR(ArrayListAppend(a, b->arr[bi++]));
    return -RERR_OK;
}

__attribute__((always_inline))
static inline int merge_clone(const ArrayList *a, const ArrayList *b, ArrayList *c) {
    int err;
    unsigned int ai = 0, bi = 0;
    while (ai < a->size && bi < b->size)
        if (a->arr[ai] >= b->arr[bi]) {
            IFERR(ArrayListAppend(c, b->arr[bi++]));
        } else {
            IFERR(ArrayListAppend(c, a->arr[ai++]));
        }
    while (ai < a->size)
        IFERR(ArrayListAppend(c, a->arr[ai++]));
    while (bi < b->size)
        IFERR(ArrayListAppend(c, b->arr[bi++]));
    return -RERR_OK;
}

int MergeList(ArrayList *a, const ArrayList *b, Nullable ArrayList *res) {
    if (res)
        return merge_clone((const ArrayList *)a, b, res);
    else
        return merge_in_place(a, b);
}

int Purge(ArrayList *list) {
    ArrayList buf;
    int err;
    IFERR(ArrayListInit(&buf, list->size)); // could only be shorter
    for (unsigned int i = 0; i < list->size; i++)
        if (ArrayListFind(&buf, list->arr[i]) == -RERR_NOTFOUND)
            IFERR(ArrayListAppend(&buf, list->arr[i]));
    free(list->arr);
    list->arr = buf.arr;
    list->size = buf.size;
    list->capacity = buf.capacity;
    return -RERR_OK;
}

int ArrayListBinaryFind(const ArrayList *list, NODE_TYPE target, Nullable unsigned *cmpTimes) {
    register unsigned size = list->size;
    int li = 0, ri = size - 1, mi = size / 2;
    NODE_TYPE *arr = list->arr;
    unsigned cmp_times = 0;
    while (li <= ri) {
        cmp_times++;
        if (target == arr[mi]) {
            if (cmpTimes)
                *cmpTimes = cmp_times;
            return mi;
        } else if (target > arr[mi])
            li = mi + 1;
        else
            ri = mi - 1;
        mi = (li + ri) / 2;
    }
    if (cmpTimes)
        *cmpTimes = cmp_times;
    return -RERR_NOTFOUND;
}

int ArrayListLinearFind(const ArrayList *list, NODE_TYPE target, Nullable unsigned *cmpTimes) {
    int err = ArrayListFind(list, target);
    if (cmpTimes) {
        if (err == -RERR_NOTFOUND)
            *cmpTimes = list->size;
        else if (err >= 0)
            *cmpTimes = err + 1;
    }
    return err;
}
#undef IFERR
