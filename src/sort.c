/*
 * Most of this file is adapted from https://github.com/zhouxumeng213/sort
 */
#include "sort.h"

#include <stdio.h>
#include <stdlib.h>

#include "arraylist.h"
#include "rerror.h"

static void bubble(NODE_TYPE *arr, unsigned size, unsigned *cmp, unsigned *swi);
static void shell(NODE_TYPE *arr, unsigned size, unsigned *cmp, unsigned *swi);
static void insert(NODE_TYPE *arr, unsigned size, unsigned *cmp, unsigned *swi);
static void selection(NODE_TYPE *arr, unsigned size, unsigned *cmp, unsigned *swi);
static void quick(NODE_TYPE *arr, int left, int right, register unsigned *cmp, register unsigned *swi);
static void heap(NODE_TYPE *arr, unsigned size, unsigned *cmp, unsigned *swi);

static unsigned qsize;
static unsigned qtime;

SortStats SortArrayList(SortType type, ArrayList *list) {
    unsigned cmp = 0, swi = 0;
    switch (type) {
        case SORT_BUBBLE:
            bubble(list->arr, list->size, &cmp, &swi);
            break;
        case SORT_SHELL:
            shell(list->arr, list->size, &cmp, &swi);
            break;
        case SORT_INSERT:
            insert(list->arr, list->size, &cmp, &swi);
            break;
        case SORT_SELECT:
            selection(list->arr, list->size, &cmp, &swi);
            break;
        case SORT_QUICK:
            qsize = list->size;
            qtime = 0;
            quick(list->arr, 0, list->size - 1, &cmp, &swi);
            break;
        case SORT_HEAP:
            heap(list->arr, list->size, &cmp, &swi);
            break;
        default:
            OUT_OF_ENUM("Illegal SortType: not implemented");
    }
    return (SortStats){cmp, swi};
}

static void insert(NODE_TYPE *arr, unsigned size, unsigned *cmp, unsigned *swi) {
    register unsigned ccnt = 0;
    register unsigned scnt = 0;
    for (register unsigned i = 1; i < size; i++) {
        NODE_TYPE tmp = arr[i];
        register unsigned j = i;
        while (j > 0 && (ccnt++, arr[j - 1] > tmp)) {
            arr[j] = arr[j - 1];
            scnt++;
            j--;
        }
        arr[j] = tmp;
        scnt++;
    }
    *cmp = ccnt;
    *swi = scnt;
}

static void shell(NODE_TYPE *arr, unsigned size, unsigned *cmp, unsigned *swi) {
    register unsigned ccnt = 0;
    register unsigned scnt = 0;
    for (register int gap = (size + 1) >> 1; gap > 0; gap >>= 1) {
        for (register unsigned i = gap; i < size; i++) {
            NODE_TYPE tmp = arr[i];
            register int j = i;
            while (j - gap >= 0 && (ccnt++, arr[j - gap] > tmp)) {
                arr[j] = arr[j - gap];
                scnt++;
                j -= gap;
            }
            arr[j] = tmp;
            scnt++;
        }
        PrintArrayList(arr, size, "After sort with gap %d", gap);
    }
    *cmp = ccnt;
    *swi = scnt;
}

static void bubble(NODE_TYPE *arr, unsigned size, unsigned *cmp, unsigned *swi) {
    register unsigned ccnt = 0;
    register unsigned scnt = 0;
    register int swapped = 1;
    for (register int i = (int)size - 2; i >= 0 && swapped; i--) {
        swapped = 0;
        for (register int j = 0; j <= i; j++)
            if (ccnt++, arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
                scnt++;
                swapped = 1;
            }
    }
    *cmp = ccnt;
    *swi = scnt;
}

static void selection(NODE_TYPE *arr, unsigned size, unsigned *cmp, unsigned *swi) {
    register unsigned ccnt = 0;
    register unsigned scnt = 0;
    for (register unsigned i = 0; i < size; i++) {
        register unsigned k = i;
        for (register unsigned j = i + 1; j < size; j++)
            if (ccnt++, arr[k] < arr[j])
                k = j;
        if (k != i) {
            int tmp = arr[k];
            arr[k] = arr[i];
            arr[i] = tmp;
            scnt++;
        }
    }
    *cmp = ccnt;
    *swi = scnt;
}

static void quick(NODE_TYPE *arr, int left, int right,
        register unsigned *cmp, register unsigned *swi) {
    if (left >= right)
        return;
    NODE_TYPE key = arr[left];
    register int begin = left;
    register int end = right;
    while (begin != end) {
        while (begin < end && (++*cmp, arr[end] >= key))
            end--;
        if (end > begin) {
            arr[begin] = arr[end];
            ++*swi;
        }
        while (begin < end && (++*cmp, arr[begin] <= key))
            begin++;
        if (begin < end) {
            arr[end] = arr[begin];
            ++*swi;
        }
    }
    arr[begin] = key;
    ++*swi;
    PrintArrayList(arr, qsize, "Slicing %d @ %d #%d", key, begin + 1, qtime++); // stupid +1 to meet textbook
    quick(arr, left, begin - 1, cmp, swi);
    quick(arr, begin + 1, right, cmp, swi);
}

#define swap(a, b) \
    NODE_TYPE t = b; \
    b = a; \
    a = t; \
    ++*swi;

static void adjust_down(NODE_TYPE *arr, unsigned root, unsigned size, register unsigned *cmp, register unsigned *swi) {
    register unsigned parent = root;
    register unsigned left = root * 2 + 1;
    while (left < size) {
        if (left + 1 < size && (++*cmp, arr[left + 1] > arr[left]))
            left++;
        if (++*cmp, arr[left] <= arr[parent])
            break;
        swap(arr[left], arr[parent]); // ++*swi
        parent = left;
        left = parent * 2 + 1;
    }
    if (parent != root)
        ++*swi; // at least it swap once. we need to add one to make it "compatible" with textbook
}

static void heap(NODE_TYPE *arr, unsigned size, register unsigned *cmp, register unsigned *swi) {
    for (register int begin = (int)size / 2 - 1; begin >= 0; --begin)
        adjust_down(arr, begin, size, cmp, swi);
    PrintArrayList(arr, size, "Initial heap");
    for (register int end = (int)size - 1; end; --end) {
        swap(arr[0], arr[end]); // ++*swi
        adjust_down(arr, 0, end, cmp, swi);
        PrintArrayList(arr, size, "Sorting #%d", size - end);
    }
}
