#include "../src/linkedlist.h"
#include "../src/listoperation.h"
#include <stdio.h>

LinkedList a;
LinkedList b;

static void tst_linked_append(void) {
    puts("=====tst_linked_append=====");
    int code = 0;
    LinkedListInit(&a);
    LinkedListInit(&b);

    code += LinkedListAppend(&a, 2);
    code += LinkedListAppend(&a, 8);
    code += LinkedListAppend(&a, 27);
    code += LinkedListAppend(&a, 39);
    code += LinkedListAppend(&a, 66);
    code += LinkedListAppend(&a, 77);
    code += LinkedListAppend(&a, 89);

    code += LinkedListAppend(&b, 6);
    code += LinkedListAppend(&b, 18);
    code += LinkedListAppend(&b, 27);
    code += LinkedListAppend(&b, 59);
    code += LinkedListAppend(&b, 65);
    code += LinkedListAppend(&b, 77);
    code += LinkedListAppend(&b, 89);
    code += LinkedListAppend(&b, 120);
    code += LinkedListAppend(&b, 140);

    printf("append operation has a last sum status: %d.\n", code);
    puts("===========================\n");
}

static void itrprint(unsigned int i, NODE_TYPE v) {
    printf("%d -> ", v);
}

static void print_list(const char *listName, const LinkedList *list) {
    printf("LinkedList %s: ", listName);
    LinkedListTraverse((LinkedList *)list, itrprint);
    puts("NULL");
}

static void tst_linked_iter(void) {
    puts("=====tst_linked_iter=====");
    print_list("a", &a);
    print_list("b", &b);
    puts("=========================\n");
}

static void tst_linked_set(void) {
    puts("=====tst_linked_set=====");
    LinkedList cp;
    LinkedListInit(&cp);
    int code = LinkedListExtend(&cp, &a);
    printf("Copying list a to cp with status %d:\n", code);
    print_list("cp", &cp);

    code = Union(&cp, &b, NULL);
    printf("Union(cp, b, NULL) with status %d:\n", code);
    print_list("cp", &cp);

    LinkedList buf;
    LinkedListInit(&buf);

    code = Intersection(&a, &b, &buf);
    printf("Intersection(a, b, buf) with status %d:\n", code);
    print_list("buf", &buf);
    
    LinkedListClear(&buf);
    code = Difference(&a, &b, &buf);
    printf("Difference(a, b, buf) with status %d:\n", code);
    print_list("buf", &buf);

    LinkedListClear(&cp);
    LinkedListClear(&buf);
    puts("========================\n");
}

static LinkedList c;

static void tst_linked_merge(void) {
    puts("=====tst_linked_merge=====");
    LinkedListInit(&c);
    int code = MergeList(&a, &b, &c);
    printf("MergeList(a, b, c) with status %d:\n", code);
    print_list("c", &c);
    puts("==========================\n");
}

static void tst_linked_purge(void) {
    puts("=====tst_linked_purge=====");
    int code = Purge(&c);
    printf("Purge(c) with status %d:\n", code);
    print_list("c", &c);
    puts("==========================\n");
}

int main(void) {
    tst_linked_append();
    tst_linked_iter();
    tst_linked_set();
    tst_linked_merge();
    tst_linked_purge();
    return 0;
}
