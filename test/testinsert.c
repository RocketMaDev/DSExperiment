#include "../src/arraylist.h"
#include "../src/sort.h"
#include "testdata.c"
#include <stdio.h>

ArrayList x, *list = &x;

static void tst_sort_by_insert(void) {
    puts("=====tst_sort_by_insert=====");
    SortStats stat = SortArrayList(SORT_INSERT, list);
    PrintArrayList(x.arr, x.size, "Sorted");
    printf("comparisons: %d, switches: %d\n", stat.ncompar, stat.nswitch);
    ArrayListClear(list);
    ArrayListExtend(list, &tmp);
    puts("============================\n");
}

static void tst_sort_by_shell(void) {
    puts("=====tst_sort_by_shell=====");
    SortStats stat = SortArrayList(SORT_SHELL, list);
    PrintArrayList(x.arr, x.size, "Sorted");
    printf("comparisons: %d, switches: %d\n", stat.ncompar, stat.nswitch);
    // no need to extend as this is the last sort
    puts("===========================\n");
}

int main(void) {
    tst_create_list();
    tst_sort_by_insert();
    tst_sort_by_shell();
    ArrayListRelease(list);
    return 0;
}
