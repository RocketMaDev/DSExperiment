#include "../src/arraylist.h"
#include "../src/arrayoperation.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static ArrayList x;
static ArrayList *list = &x;

#include "testdata.c"

static void pitem(unsigned i, int v, void *buf) {
    (void)i, (void)buf;
    printf("|%3d|%6s|%6s|%3d|\n", v, MAP(v)->name, MAP(v)->female ? "Female" : "Male", MAP(v)->age);
}

static void tst_create_list(void) {
    puts("=====tst_create_list=====");
    ArrayList tmp = {(int *)keys, ninfo, ninfo};
    ArrayListInit(list, ninfo);
    int err = ArrayListExtend(list, &tmp);
    printf("Created search list with status %d\n", err);
    puts("Info table: [");
    printf("|%3s|%6s|%6s|%3s|\n", "key", "name", "sex", "age");
    ArrayListTraverse(list, NULL, pitem);
    puts("]");
    puts("=========================\n");
}

static int keysToSearch[] = {56, 19, 80, 5, 21, 64, 89, 13, 38, 75, 92, 4, 57, 20, 81, 6, 22, 65, 90, 14, 39, 120, 93};
static unsigned searchs = sizeof(keysToSearch) / sizeof(int);

static void search_template(const char *searchType, int (*func)(const ArrayList *, int, unsigned *)) {
    unsigned cur, nfsum = 0, fsum = 0, nftime = 0, ftime = 0;
    for (unsigned i = 0; i < searchs; i++) {
        int key = keysToSearch[i];
        int err = func(list, key, &cur);
        assert(err >= 0 || err == -RERR_NOTFOUND);
        if (err != -RERR_NOTFOUND) {
            printf("Found record #%d with %d times of comparison: %s, %c, %d\n",
                    key, cur, MAP(key)->name, MAP(key)->female ? 'F' : 'M', MAP(key)->age);
            fsum += cur;
            ftime++;
        } else {
            printf("Record #%d not found after %d times of comparison\n", key, cur);
            nfsum += cur;
            nftime++;
        }
    }
    printf("ASL of successful %s search: %.2lf\n", searchType, (double)fsum / ftime);
    printf("ASL of failed %s search: %.2lf\n", searchType, (double)nfsum / nftime);
}

static void tst_linear_search(void) {
    puts("=====tst_linear_search=====");
    search_template("linear", ArrayListLinearFind);
    puts("===========================\n");
}

static int cmp(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

static void tst_sort(void) {
    puts("=====tst_sort=====");
    qsort(x.arr, x.size, sizeof(int), cmp);
    puts("Sorted info table: [");
    printf("|%3s|%6s|%6s|%3s|\n", "key", "name", "sex", "age");
    ArrayListTraverse(list, NULL, pitem);
    puts("]");
    puts("==================\n");
}

static void tst_binary_search(void) {
    puts("=====tst_binary_search=====");
    search_template("binary", ArrayListBinaryFind);
    puts("===========================\n");
}

int main(void) {
    tst_create_list();
    tst_linear_search();
    tst_sort();
    tst_binary_search();
    ArrayListRelease(list);
    return 0;
}
