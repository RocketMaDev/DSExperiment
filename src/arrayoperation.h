#ifndef __ARRAY_OPERATION_H__
#define __ARRAY_OPERATION_H__

#include "arraylist.h"
int Union(ArrayList *a, const ArrayList *b, Nullable ArrayList *ans);
int Intersection(ArrayList *a, const ArrayList *b, Nullable ArrayList *ans);
int Difference(ArrayList *a, const ArrayList *b, Nullable ArrayList *ans);

int MergeList(ArrayList *a, const ArrayList *b, Nullable ArrayList *res);

int Purge(ArrayList *list);
#endif
