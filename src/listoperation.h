#ifndef __LIST_OPERATION_H__
#define __LIST_OPERATION_H__

#include "linkedlist.h"
int Union(LinkedList *a, const LinkedList *b, Nullable LinkedList *ans);
int Intersection(LinkedList *a, const LinkedList *b, Nullable LinkedList *ans);
int Difference(LinkedList *a, const LinkedList *b, Nullable LinkedList *ans);

int MergeList(LinkedList *a, const LinkedList *b, Nullable LinkedList *res);

int Purge(LinkedList *list);
#endif
