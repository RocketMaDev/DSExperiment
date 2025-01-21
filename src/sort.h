/**
 * Chapter 8 - SORT
 * Experiment 5.1 - Insertion Sort
 * Experiment 5.2 - Exchange Sort
 * Experiment 5.3 - Selection Sort
 */
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

/**
 * Sort the array list with given method.
 *
 * @param type One of sort types.
 * @param list The list to sort.
 * @return A struct of comparison times and element switch times.
 */
SortStats SortArrayList(SortType type, ArrayList *list);

#ifdef DETAILED
/**
 * Print an array list by `arr` and `size` with style like
 * `MESSAGE: [1, 2, 3]`, where `MESSAGE` will be injected by formatted text.
 * The arg `fmt` and args following are treated as format string and
 * contribute to `MESSAGE`.
 *
 * @param arr   `arr` in array list.
 * @param size  `size` in array list.
 * @param fmt   Format for `MESSAGE`.
 * @param ...   Vaargs to inject into `fmt` like `printf`.
 *
 * @note This function should be declared in your test file.
 */
extern void PrintArrayList(const NODE_TYPE *arr, unsigned size, const char *fmt, ...);
#else
#define PrintArrayList(arr, size, fmt, ...)
#endif
#endif
