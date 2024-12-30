#ifndef __ARRAY_OPERATION_H__
#define __ARRAY_OPERATION_H__

#include "rerror.h"
#include "arraylist.h"
int Union(ArrayList *a, const ArrayList *b, Nullable ArrayList *ans);
int Intersection(ArrayList *a, const ArrayList *b, Nullable ArrayList *ans);
int Difference(ArrayList *a, const ArrayList *b, Nullable ArrayList *ans);

int MergeList(ArrayList *a, const ArrayList *b, Nullable ArrayList *res);

int Purge(ArrayList *list);

int ArrayListLinearFind(const ArrayList *list, NODE_TYPE target, Nullable unsigned *cmpTimes);
int ArrayListBinaryFind(const ArrayList *list, NODE_TYPE target, Nullable unsigned *cmpTimes);
#endif
