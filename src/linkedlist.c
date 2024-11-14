#include "linkedlist.h"
#include <stdlib.h>

void LinkedListInit(LinkedList *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void LinkedListClear(LinkedList *list) {
    if (!list->head)
        return;
    NODE *prev = list->head;
    while (prev->next) {
        NODE *tmp = prev;
        prev = prev->next;
        free(tmp);
    }
    free(prev);
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void LinkedListRelease(LinkedList *list) {
    if (list->size)
        LinkedListClear(list);
}

int LinkedListAppend(LinkedList *list, NODE_TYPE v) {
    if (__builtin_expect(list->size >= MAX_SIZE, 0))
        return -LLERR_OVERFLOW;
    NODE *node = malloc(sizeof(NODE));
#define IFOOM(node) \
    if (__builtin_expect(!node, 0))  \
        return -LLERR_OOM
    IFOOM(node);
    if (!list->head) {
        list->head = list->tail = node;
        node->next = NULL;
        node->value = v;
    } else {
        list->tail->next = node;
        list->tail = node;
        node->next = NULL;
        node->value = v;
    }
    list->size++;
    return -LLERR_OK;
}

int LinkedListInsert(LinkedList *list, unsigned int index, NODE_TYPE v) {
    if (index > list->size)
        return -LLERR_OVERFLOW;
    if (index == list->size)
        return LinkedListAppend(list, v);
    if (index == 0)
        return LinkedListPrepend(list, v);
    NODE *prev = list->head;
    NODE *np = prev->next;
    for (unsigned int i = 1; i < index; i++) {
        prev = np;
        np = np->next;
    }
    NODE *node = (NODE *)malloc(sizeof(NODE));
    IFOOM(node);
    node->next = np;
    node->value = v;
    prev->next = node;
    return -LLERR_OK;
}

int LinkedListPrepend(LinkedList *list, NODE_TYPE v) {
    if (__builtin_expect(list->size >= MAX_SIZE, 0))
        return -LLERR_OVERFLOW;
    NODE *node = malloc(sizeof(NODE));
    IFOOM(node);
    if (!list->head) {
        list->head = list->tail = node;
        node->next = NULL;
        node->value = v;
    } else {
        node->next = list->head;
        list->head = node;
        node->value = v;
    }
    list->size++;
    return -LLERR_OK;
}

int LinkedListDelete(LinkedList *list, unsigned int index, NODE_TYPE *recv) {
    if (__builtin_expect(list->size >= MAX_SIZE, 0))
        return -LLERR_OVERFLOW;
    NODE_TYPE discard;
    if (!recv)
        recv = &discard;
    if (index == 0) {
        *recv = list->head->value;
        if (list->size == 1) {
            free(list->head);
            list->head = 0;
            list->tail = 0;
        } else {
            NODE *next = list->head->next;
            free(list->head);
            list->head = next;
        }
        list->size--;
        return -LLERR_OK;
    }
    NODE *node = list->head;
    for (unsigned int i = 0; i < index - 1; i++)
        node = node->next;
    *recv = node->next->value;
    if (index == list->size - 1) {
        list->tail = node;
        free(node->next);
        node->next = NULL;
    } else {
        NODE *next = node->next->next;
        free(node->next);
        node->next = next;
    }
    list->size--;
    return -LLERR_OK;
}

int LinkedListSet(LinkedList *list, unsigned int index, NODE_TYPE v) {
    if (index >= list->size)
        return -LLERR_OVERFLOW;
    if (index == list->size - 1) {
        list->tail->value = v;
        return -LLERR_OK;
    }
    NODE *node = list->head;
    for (unsigned int i = 0; i < index; i++)
        node = node->next;
    node->value = v;
    return -LLERR_OK;
}

int LinkedListGet(const LinkedList *list, unsigned int index, NODE_TYPE *recv) {
    NODE_TYPE discard;
    if (!recv)
        recv = &discard;
    if (index >= list->size)
        return -LLERR_OVERFLOW;
    if (index == list->size - 1) {
        *recv = list->tail->value;
        return -LLERR_OK;
    }
    NODE *node = list->head;
    for (unsigned int i = 0; i < index; i++)
        node = node->next;
    *recv = node->value;
    return -LLERR_OK;
}

unsigned int LinkedListSize(const LinkedList *list) {
    return list->size;
}

int LinkedListFind(const LinkedList *list, NODE_TYPE v) {
    if (!list->head)
        return -LLERR_NOTFOUND;
    NODE *node = list->head;
    unsigned int idx = 0;
    do {
#ifndef OVERRIDE_CMP
        if (node->value == v)
#else
        if (NodeCmp(node->value, v))
#endif
            return idx;
        idx++;
        node = node->next;
    } while (node);
    return -LLERR_NOTFOUND;
}

int LinkedListRemove(LinkedList *list, NODE_TYPE v) {
    int idx;
    NODE *node, *tofree;
    unsigned int count;
    if ((idx = LinkedListFind(list, v)) < 0)
        return -LLERR_NOTFOUND;
    count = 0;
    while (idx >= 0) {
        if (idx == 0) {
            tofree = list->head;
            node = tofree->next;
            free(tofree);
            list->head = node;
        } else {
            node = list->head;
            // idx - 1 to stop at the node which we want to remove
            for (int i = 0; i < idx - 1; i++)
                node = node->next;
            tofree = node->next;
            node->next = tofree->next;
            free(tofree);
        }
        if (tofree == list->tail) {
            if (!list->head) 
                list->tail = NULL;
            else
                list->tail = node;
        }
        list->size--;
        count++;
        idx = LinkedListFind(list, v);
    }
    return count;
}

void LinkedListTraverse(LinkedList *list, void (*func)(unsigned int, NODE_TYPE)) {
    if (!list->head)
        return;
    NODE *node = list->head;
    unsigned int index = 0;
    do {
        func(index++, node->value);
        node = node->next;
    } while (node);
}

static NODE_TYPE *valsToCopy;
static void cp_vals(unsigned int index, NODE_TYPE v) {
    valsToCopy[index] = v;
}

int LinkedListExtend(LinkedList *dst, const LinkedList *src) {
    if (!src->size)
        return -LLERR_OK;
    LinkedList buf;
    LinkedListInit(&buf);
    valsToCopy = (NODE_TYPE *)malloc(sizeof(NODE_TYPE) * src->size);
    if (!valsToCopy)
        return -LLERR_OOM;
    LinkedListTraverse((LinkedList *)src, cp_vals);
    for (unsigned int i = 0; i < src->size; i++)
        if (__builtin_expect(LinkedListAppend(&buf, valsToCopy[i]) < 0, 0))
            goto cleanup;
    if (!dst->head) { // if dst is empty, we are performing a clone
        dst->head = buf.head;
        dst->tail = buf.tail;
        dst->size = buf.size;
        return -LLERR_OK;
    }
    dst->tail->next = buf.head;
    dst->size += buf.size;
    dst->tail = buf.tail;
    return -LLERR_OK;
cleanup:
    LinkedListClear(&buf);
    return -LLERR_OOM;
}
#undef IFOOM
