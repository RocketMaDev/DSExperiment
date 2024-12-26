#include "../src/str.h"
#include <assert.h>
#include <stdio.h>

static String t1, t2, t3, s;
static void tst_assign(void) {
    puts("=====tst_assign=====");
    StrAssign(&t1, "<HTTP_METHOD>");
    printf("Constructing t1: %s\n", t1.arr);
    StrAssign(&t2, "<REQUEST_TARGET>");
    printf("Constructing t2: %s\n", t2.arr);
    StrAssign(&t3, "<HTTP_VERSION>");
    printf("Constructing t3: %s\n", t3.arr);
    puts("====================\n");
}

static void put(unsigned int i, unsigned char v, void *buf) {
    (void)i, (void)buf;
    putchar(v);
}

static void tst_traverse(void) {
    puts("=====tst_traverse=====");
    printf("Traversing t1 with putchar: ");
    StrTraverse(&t1, NULL, put);
    puts("");
    puts("======================\n");
}

static void tst_clone(void) {
    puts("=====tst_clone=====");
    StrClone(&s, &t1);
    printf("Constructing s with t1: %s\n", s.arr);
    puts("===================\n");
}

static void tst_insert(void) {
    puts("=====tst_insert=====");
    StrInsert(&s, s.size, &t2);
    printf("After appending t2 to s: %s\n", s.arr);
    puts("====================\n");
}

static void tst_replace(void) {
    puts("=====tst_replace=====");
    StrReplace(&s, &t1, &t3);
    printf("After replacing t1 with t3 in s: %s\n", s.arr);
    puts("=====================\n");
}

static void tst_index(void) {
    puts("=====tst_index=====");
    printf("Search t2 in s: %d\n", StrIndex(&s, &t2, 0));
    puts("===================\n");
}

static void tst_delete(void) {
    puts("=====tst_delete=====");
    int idx = StrIndex(&s, &t2, 0);
    assert(idx >= 0);
    StrDelete(&s, idx, t2.size);
    printf("After deleting t2 in s: %s\n", s.arr);
    puts("====================\n");
}

int main(void) {
    tst_assign();
    tst_traverse();
    tst_clone();
    tst_insert();
    tst_replace();
    tst_index();
    tst_delete();
    ArrayListRelease(&t1);
    ArrayListRelease(&t2);
    ArrayListRelease(&t3);
    ArrayListRelease(&s);
    return 0;
}
