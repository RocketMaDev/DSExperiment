#include "../src/arraylist.h"
#include "../src/arrayoperation.h"
#include <stdio.h>

ArrayList a;
ArrayList b;

static void print_list(const char *listName, const ArrayList *list) {
    printf("ArrayList %s: [", listName);
    for (unsigned int i = 0; i < list->size - 1; i++)
        printf("%d, ", list->arr[i]);
    printf("%d]\n", list->arr[list->size - 1]);
}

static void tst_array_append(void) {
    puts("=====tst_array_append=====");
    int code = 0;
    ArrayListInit(&a, 0);
    ArrayListInit(&b, 0);

    code += ArrayListAppend(&a, 2);
    code += ArrayListAppend(&a, 8);
    code += ArrayListAppend(&a, 27);
    code += ArrayListAppend(&a, 39);
    code += ArrayListAppend(&a, 66);
    code += ArrayListAppend(&a, 77);
    code += ArrayListAppend(&a, 89);

    code += ArrayListAppend(&b, 6);
    code += ArrayListAppend(&b, 18);
    code += ArrayListAppend(&b, 27);
    code += ArrayListAppend(&b, 59);
    code += ArrayListAppend(&b, 65);
    code += ArrayListAppend(&b, 77);
    code += ArrayListAppend(&b, 89);
    code += ArrayListAppend(&b, 120);
    code += ArrayListAppend(&b, 140);

    printf("append operation has a last sum status: %d.\n", code);
    print_list("a", &a);
    print_list("b", &b);
    puts("==========================\n");
}

static void tst_array_set(void) {
    puts("=====tst_array_set=====");
    ArrayList cp;
    ArrayListInit(&cp, a.capacity);
    int code = ArrayListExtend(&cp, &a);
    printf("Copying list a to cp with status %d:\n", code);
    print_list("cp", &cp);

    code = Union(&cp, &b, NULL);
    printf("Union(cp, b, NULL) with status %d:\n", code);
    print_list("cp", &cp);

    ArrayList buf;
    ArrayListInit(&buf, 0);

    code = Intersection(&a, &b, &buf);
    printf("Intersection(a, b, buf) with status %d:\n", code);
    print_list("buf", &buf);
    
    ArrayListClear(&buf);
    code = Difference(&a, &b, &buf);
    printf("Difference(a, b, buf) with status %d:\n", code);
    print_list("buf", &buf);

    ArrayListRelease(&cp);
    ArrayListRelease(&buf);
    puts("=======================\n");
}

static ArrayList c;

static void tst_array_merge(void) {
    puts("=====tst_array_merge=====");
    ArrayListInit(&c, a.size + b.size);
    int code = MergeList(&a, &b, &c);
    printf("MergeList(a, b, c) with status %d:\n", code);
    print_list("c", &c);
    puts("=========================\n");
}

static void tst_array_purge(void) {
    puts("=====tst_array_purge=====");
    int code = Purge(&c);
    printf("Purge(c) with status %d:\n", code);
    print_list("c", &c);
    puts("=========================\n");
}

int main(void) {
    tst_array_append();
    tst_array_set();
    tst_array_merge();
    tst_array_purge();
    return 0;
}
