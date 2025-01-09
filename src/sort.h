#ifndef __SORT_H__
#define __SORT_H__
#include "arraylist.h"
typedef enum {
    SORT_INSERT,
    SORT_SHELL,
    SORT_BUBBLE,
    SORT_QUICK,
    SORT_SELECT,
    SORT_HEAP,
} SortType;

typedef struct {
    unsigned ncompar;
    unsigned nswitch;
} SortStats;

SortStats SortArrayList(SortType type, ArrayList *list);

#ifdef DETAILED
extern void PrintArrayList(const NODE_TYPE *arr, unsigned size, const char *fmt, ...);
#else
#define PrintArrayList(arr, size, fmt, ...)
#endif
#endif
