#include "../src/arraylist.h"
#include "../src/hashtable.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static LinearHashTable table, *ptable = &table;
static LinkedHashTable links, *plinks = &links;
static int buf[] = {19, 1, 20, 84, 11, 79, 26, 40, 29, 30, 60, 74, 38};
static ArrayList keys = {
    buf, sizeof(buf) / sizeof(int), sizeof(buf) / sizeof(int)
};

static size_t hasher(int v) {
    return v % 13;
}

static void insert_table(unsigned i, int v, void *buf) {
    (void)i, (void)buf;
    int err = LinearHashTableInsert(ptable, v, hasher);
    err += LinkedHashTableInsert(plinks, v, hasher);
    if (err < 0)
        OUT_OF_ENUM("Failed to insert hash ptable!");
}

static void plink(unsigned i, int v) {
    (void)i;
    printf("%d -> ", v);
}

static void tst_create_tables(void) {
    puts("=====tst_create_tables=====");
    int err = LinearHashTableInit(ptable, 16);
    err += LinkedHashTableInit(plinks, 13);
    printf("Initialized 2 hash tables with status %d\n", err);
    int buf[] = {19, 14, 23, 1, 68, 20, 84, 27, 55, 11, 10, 79};
    ArrayList tmp = {buf, sizeof(buf) / sizeof(int), sizeof(buf) / sizeof(int)};
    ArrayListTraverse(&tmp, NULL, insert_table);

    printf("LinearHashTable: [");
    for (unsigned i = 0; i < table.size - 1; i++)
        if (table.used & (1 << i))
            printf("%d, ", table.arr[i]);
        else
            printf("?, ");
    if (table.used & (1 << (table.size - 1)))
        printf("%d]\n", table.arr[table.size - 1]);
    else
        printf("?]\n");

    puts("LinkedHashTable: ");
    for (unsigned i = 0; i < links.size; i++) {
        printf("%d: ", i);
        LinkedListTraverse(links.buckets + i, plink);
        puts("NULL");
    }
    puts("===========================\n");
}

typedef int (*find_func_t)(const void *, int, hashfunc_t, unsigned *);
static void search_template(const char *searchType, const void *table, find_func_t func) {
    unsigned fsum = 0, fcnt = 0, nfsum = 0, nfcnt = 0;
    unsigned cur;
    for (unsigned i = 0; i < keys.size; i++) {
        int key = keys.arr[i];
        int err = func(table, key, hasher, &cur);
        assert(err >= 0 || err == -RERR_NOTFOUND);
        if (err != -RERR_NOTFOUND) {
            printf("Found key %d @ %d with %d times of comparison\n",
                    key, err, cur);
            fsum += cur;
            fcnt++;
        } else {
            printf("Key %d not found with %d times of comparison\n",
                    key, cur);
            nfsum += cur;
            nfcnt++;
        }
    }
    printf("ASL of successful %s search: %.2lf\n", searchType, (double)fsum / fcnt);
    printf("ASL of failed %s search: %.2lf\n", searchType, (double)nfsum / nfcnt);
}

static void tst_linear_table(void) {
    puts("=====tst_linear_table=====");
    search_template("LinearHashTable", ptable, (find_func_t)LinearHashTableFind);
    puts("==========================\n");
}

static void tst_linked_table(void) {
    puts("=====tst_linked_table=====");
    search_template("LinkedHashTable", plinks, (find_func_t)LinkedHashTableFind);
    puts("==========================\n");
}

int main(void) {
    tst_create_tables();
    tst_linear_table();
    tst_linked_table();
    LinearHashTableRelease(ptable);
    LinkedHashTableRelease(plinks);
    return 0;
}
