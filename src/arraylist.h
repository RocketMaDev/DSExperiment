#ifndef __ARRAY_LIST_H__
#define __ARRAY_LIST_H__
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

typedef struct {
    NODE_TYPE *arr;
    unsigned int size;
    unsigned int capacity;
} ArrayList;

int ArrayListInit(ArrayList *list, unsigned int initSize);
void ArrayListClear(ArrayList *list);
void ArrayListRelease(ArrayList *list);

int ArrayListAppend(ArrayList *list, NODE_TYPE v);
int ArrayListInsert(ArrayList *list, unsigned int index, NODE_TYPE v);
int ArrayListPrepend(ArrayList *list, NODE_TYPE v);
int ArrayListDelete(ArrayList *list, unsigned int index, Nullable NODE_TYPE *recv);

int ArrayListSet(ArrayList *list, unsigned int index, NODE_TYPE v);
int ArrayListGet(const ArrayList *list, unsigned int index, Nullable NODE_TYPE *recv);

unsigned int ArrayListSize(const ArrayList *list);

int ArrayListFind(const ArrayList *list, NODE_TYPE v);
int ArrayListRemove(ArrayList *list, NODE_TYPE v);

void ArrayListTraverse(ArrayList *list, void (*func)(unsigned int index, NODE_TYPE v));
int ArrayListExtend(ArrayList *dst, const ArrayList *src);
#endif

