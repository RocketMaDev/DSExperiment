#include "listoperation.h"
#include "linkedlist.h"
#include "rerror.h"
#include <assert.h>
#include <stdlib.h>

static void cp_vals1(unsigned int index, NODE_TYPE v, void *buf) {
    ((NODE_TYPE *)buf)[index] = v;
}

int Union(LinkedList *a, const LinkedList *b, LinkedList *ans) {
    if (!a->size)
        return LinkedListExtend(ans ? ans : a, b);
    if (!b->size) {
        if (ans)
            return LinkedListExtend(ans, a);
        return -RERR_OK;
    }
    NODE_TYPE *gbuf1 = (NODE_TYPE *)malloc(sizeof(NODE_TYPE) * b->size);
    int err = -RERR_OOM;
    if (!gbuf1)
        goto cleanup;
    LinkedListTraverse((LinkedList *)b, gbuf1, cp_vals1);
    
    if (ans && LinkedListExtend(ans, a) < 0)
        goto cleanup;
    LinkedList *target = ans ? ans : a;
    for (unsigned int i = 0; i < b->size; i++)
        LinkedListRemove(target, gbuf1[i]);
    err = LinkedListExtend(target, b);
cleanup:
    free(gbuf1);
    return err;
}

int Intersection(LinkedList *a, const LinkedList *b, LinkedList *ans) {
    if (!a->size)
        return -RERR_OK;
    if (!b->size) {
        LinkedListClear(ans ? ans : a);
        return -RERR_OK;
    }
    NODE_TYPE *gbuf1 = (NODE_TYPE *)malloc(sizeof(NODE_TYPE) * b->size);
    NODE_TYPE *gbuf2 = (NODE_TYPE *)malloc(sizeof(NODE_TYPE) * b->size);
    if (!gbuf1 || !gbuf2)
        goto cleanup;
    LinkedListTraverse((LinkedList *)b, gbuf1, cp_vals1);

    unsigned int count = 0;
    for (unsigned int i = 0; i < b->size; i++)
        if (LinkedListFind(a, gbuf1[i]) >= 0)
            gbuf2[count++] = gbuf1[i];

    if (!ans)
        LinkedListClear(a);
    LinkedList *target = ans ? ans : a;
    for (unsigned int i = 0; i < count; i++)
        if (LinkedListAppend(target, gbuf2[i]) < 0)
            goto cleanup;
    free(gbuf1);
    free(gbuf2);
    return -RERR_OK;
cleanup:
    free(gbuf1);
    free(gbuf2);
    return -RERR_OOM;
}

int Difference(LinkedList *a, const LinkedList *b, LinkedList *ans) {
    if (!a->size)
        return -RERR_OK;
    if (!b->size) {
        if (ans)
            return LinkedListExtend(ans, a);
        return -RERR_OK;
    }
    NODE_TYPE *gbuf1 = (NODE_TYPE *)malloc(sizeof(NODE_TYPE) * b->size);
    if (!gbuf1)
        goto cleanup;
    LinkedListTraverse((LinkedList *)b, gbuf1, cp_vals1);
    
    if (ans && LinkedListExtend(ans, a) < 0)
        goto cleanup;
    LinkedList *target = ans ? ans : a;
    for (unsigned int i = 0; i < b->size; i++)
        LinkedListRemove(target, gbuf1[i]);
    free(gbuf1);
    return -RERR_OK;
cleanup:
    free(gbuf1);
    return -RERR_OOM;
}

struct __dup {
    int detected;
    unsigned int count;
    unsigned int *todelete;
    NODE_TYPE t;
};

void detect_dup(unsigned int index, NODE_TYPE v, void *buf) {
    register struct __dup *dup = (struct __dup *)buf;
    if (dup->t != v)
        return;
    if (!dup->detected && (dup->detected = 1))
        return;
    dup->todelete[dup->count++] = index;
}

int Purge(LinkedList *list) {
    if (!list->head)
        return -RERR_OK;
    struct __dup dup = {};
    unsigned int *todelete = (unsigned int *)malloc(sizeof(unsigned int) * list->size);
    if (!todelete)
        return -RERR_OOM;
    dup.todelete = todelete;
    for (unsigned int i = 0; i < list->size; i++) {
        dup.detected = 0;
        assert(LinkedListGet(list, i, &dup.t) == 0);
        dup.count = 0;
        LinkedListTraverse(list, &dup, detect_dup);
        for (unsigned int j = 0; j < dup.count; j++) {
            assert(LinkedListDelete(list, todelete[j] - j, NULL) == 0);
            if (todelete[j] <= i)
                i--;
        }
    }
    free(todelete);
    return -RERR_OK;
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
        return -RERR_OK;
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
    return -RERR_OK;
cleanup:
    return -RERR_OOM;
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
    return -RERR_OK;
cleanup:
    LinkedListClear(c);
    return -RERR_OOM;
}

#undef OOM_EX
