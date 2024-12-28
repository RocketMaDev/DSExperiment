#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__
#include "rerror.h"
#ifndef NODE_TYPE
#   define NODE_TYPE int
#endif

#ifndef MAX_SIZE
#   define MAX_SIZE 1000
#endif

#define Nullable

#   ifdef OVERRIDE_CMP
typedef int (*NODE_HOOK)(NODE_TYPE a, NODE_TYPE b);
extern NODE_HOOK NodeCmp;
#   endif

typedef struct __node {
    NODE_TYPE value;
    struct __node *next;
} NODE;

typedef struct {
    NODE *head;
    NODE *tail;
    unsigned int size;
} LinkedList;

void LinkedListInit(LinkedList *list);
void LinkedListClear(LinkedList *list);
void LinkedListRelease(LinkedList *list);

int LinkedListAppend(LinkedList *list, NODE_TYPE v);
int LinkedListInsert(LinkedList *list, unsigned int index, NODE_TYPE v);
int LinkedListPrepend(LinkedList *list, NODE_TYPE v);
int LinkedListDelete(LinkedList *list, unsigned int index, Nullable NODE_TYPE *recv);

int LinkedListSet(LinkedList *list, unsigned int index, NODE_TYPE v);
int LinkedListGet(const LinkedList *list, unsigned int index, Nullable NODE_TYPE *recv);

unsigned int LinkedListSize(const LinkedList *list);

int LinkedListFind(const LinkedList *list, NODE_TYPE v);
int LinkedListRemove(LinkedList *list, NODE_TYPE v);

void LinkedListTraverse(LinkedList *list, void (*func)(unsigned int index, NODE_TYPE v));
int LinkedListExtend(LinkedList *dst, const LinkedList *src);
#endif
