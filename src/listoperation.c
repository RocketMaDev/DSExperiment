#include "listoperation.h"
#include "linkedlist.h"
#include <assert.h>
#include <stdlib.h>

static NODE_TYPE *gbuf1;
static NODE_TYPE *gbuf2;

static void cp_vals1(unsigned int index, NODE_TYPE v) {
    gbuf1[index] = v;
}

int Union(LinkedList *a, const LinkedList *b, LinkedList *ans) {
    if (!a->size)
        return LinkedListExtend(ans ? ans : a, b);
    if (!b->size) {
        if (ans)
            return LinkedListExtend(ans, a);
        return -LLERR_OK;
    }
    gbuf1 = (NODE_TYPE *)malloc(sizeof(NODE_TYPE) * b->size);
    if (!gbuf1)
        return -LLERR_OOM;
    LinkedListTraverse((LinkedList *)b, cp_vals1);
    
    if (ans && LinkedListExtend(ans, a) < 0)
        return -LLERR_OOM;
    LinkedList *target = ans ? ans : a;
    for (unsigned int i = 0; i < b->size; i++)
        LinkedListRemove(target, gbuf1[i]);
    return LinkedListExtend(target, b);
}

int Intersection(LinkedList *a, const LinkedList *b, LinkedList *ans) {
    if (!a->size)
        return -LLERR_OK;
    if (!b->size) {
        LinkedListClear(ans ? ans : a);
        return -LLERR_OK;
    }
    gbuf1 = (NODE_TYPE *)malloc(sizeof(NODE_TYPE) * b->size);
    if (!gbuf1)
        return -LLERR_OOM;
    LinkedListTraverse((LinkedList *)b, cp_vals1);

    unsigned int count = 0;
    gbuf2 = (NODE_TYPE *)malloc(sizeof(NODE_TYPE) * b->size);
    for (unsigned int i = 0; i < b->size; i++)
        if (LinkedListFind(a, gbuf1[i]) >= 0)
            gbuf2[count++] = gbuf1[i];

    if (!ans)
        LinkedListClear(a);
    LinkedList *target = ans ? ans : a;
    for (unsigned int i = 0; i < count; i++)
        if (LinkedListAppend(target, gbuf2[i]) < 0)
            return -LLERR_OOM;
    return -LLERR_OK;
}

int Difference(LinkedList *a, const LinkedList *b, LinkedList *ans) {
    if (!a->size)
        return -LLERR_OK;
    if (!b->size) {
        if (ans)
            return LinkedListExtend(ans, a);
        return -LLERR_OK;
    }
    gbuf1 = (NODE_TYPE *)malloc(sizeof(NODE_TYPE) * b->size);
    if (!gbuf1)
        return -LLERR_OOM;
    LinkedListTraverse((LinkedList *)b, cp_vals1);
    
    if (ans && LinkedListExtend(ans, a) < 0)
        return -LLERR_OOM;
    LinkedList *target = ans ? ans : a;
    for (unsigned int i = 0; i < b->size; i++)
        LinkedListRemove(target, gbuf1[i]);
    return -LLERR_OK;
}

static int detected;
static NODE_TYPE t;
static unsigned int *todelete;
static unsigned int count;

void detect_dup(unsigned int index, NODE_TYPE v) {
    if (t != v)
        return;
    if (!detected && (detected = 1))
        return;
    todelete[count++] = index;
}

int Purge(LinkedList *list) {
    if (!list->head)
        return -LLERR_OK;
    todelete = (unsigned int *)malloc(sizeof(unsigned int) * list->size);
    if (!todelete)
        return -LLERR_OOM;
    for (unsigned int i = 0; i < list->size; i++) {
        detected = 0;
        assert(LinkedListGet(list, i, &t) == 0);
        count = 0;
        LinkedListTraverse(list, detect_dup);
        for (unsigned int j = 0; j < count; j++) {
            assert(LinkedListDelete(list, todelete[j] - j, NULL) == 0);
            if (todelete[j] <= i)
                i--;
        }
    }
    return -LLERR_OK;
}

static int merge_in_place(LinkedList *, const LinkedList *);
static int merge_clone(const LinkedList *, const LinkedList *, LinkedList *);

int MergeList(LinkedList *a, const LinkedList *b, LinkedList *res) {
    if (!res)
        return merge_in_place(a, b);
    else
        return merge_clone((const LinkedList *)a, b, res);
}

#define OOM_EX(cond) \
    if (__builtin_expect(cond, 0)) \
        goto cleanup;

__attribute__((always_inline))
static inline int merge_in_place(LinkedList *a, const LinkedList *b) {
    if (a->size == 0)
        return LinkedListExtend(a, b);
    if (b->size == 0)
        return -LLERR_OK;
    NODE *prev = NULL;
    NODE *ap = a->head;
    NODE *bp = b->head;
    while (ap != NULL && bp != NULL)
        if (ap->value <= bp->value) {
            prev = ap;
            ap = ap->next;
        } else {
            NODE *tmp = (NODE *)malloc(sizeof(NODE));
            OOM_EX(tmp == NULL);
            tmp->value = bp->value;
            tmp->next = ap;
            if (prev)
                prev->next = tmp;
            else
                a->head = tmp;
            prev = tmp;
            bp = bp->next;
        }
    while (bp != NULL) {
        OOM_EX(LinkedListAppend(a, bp->value));
        bp = bp->next;
    }
    return -LLERR_OK;
cleanup:
    return -LLERR_OOM;
}

__attribute__((always_inline))
static inline int merge_clone(const LinkedList *a, const LinkedList *b, LinkedList *c) {
    NODE *ap = a->head;
    NODE *bp = b->head;
    while (ap != NULL && bp != NULL)
        if (ap->value >= bp->value) {
            OOM_EX(LinkedListAppend(c, bp->value));
            bp = bp->next;
        } else {
            OOM_EX(LinkedListAppend(c, ap->value));
            ap = ap->next;
        }
    while (ap != NULL) {
        OOM_EX(LinkedListAppend(c, ap->value));
        ap = ap->next;
    }
    while (bp != NULL) {
        OOM_EX(LinkedListAppend(c, bp->value));
        bp = bp->next;
    }
    return -LLERR_OK;
cleanup:
    LinkedListClear(c);
    return -LLERR_OOM;
}

#undef OOM_EX
