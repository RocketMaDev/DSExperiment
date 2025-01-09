#include "../src/arraylist.h"
#include "../src/sort.h"
#include "testdata.c"
#include <stdio.h>

ArrayList x, *list = &x;

static void tst_sort_by_select(void) {
    puts("=====tst_sort_by_select=====");
    SortStats stat = SortArrayList(SORT_SELECT, list);
    PrintArrayList(x.arr, x.size, "Sorted");
    printf("comparisons: %d, switches: %d\n", stat.ncompar, stat.nswitch);
    ArrayListClear(list);
    ArrayListExtend(list, &tmp);
    puts("============================\n");
}

static void tst_sort_by_heap(void) {
    puts("=====tst_sort_by_heap=====");
    SortStats stat = SortArrayList(SORT_HEAP, list);
    PrintArrayList(x.arr, x.size, "Sorted");
    printf("comparisons: %d, switches: %d\n", stat.ncompar, stat.nswitch);
    // no need to extend as this is the last sort
    puts("===========================\n");
}

int main(void) {
    tst_create_list();
    tst_sort_by_select();
    tst_sort_by_heap();
    ArrayListRelease(list);
    return 0;
}
