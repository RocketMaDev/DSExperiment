#include "testbook.h"
#include "../src/linkedlist.h"
#include <stdio.h>
#include <string.h>

#ifndef OVERRIDE_CMP
#define OVERRIDE_CMP
#endif

static int cmp(NODE_TYPE a, NODE_TYPE b) {
    int val = strcmp(a.bookid, b.bookid);
    if (val)
        return !val;
    val = strcmp(a.bookname, b.bookname);
    if (val)
        return !val;
    return a.price == b.price;
}
NODE_HOOK NodeCmp = cmp;

static LinkedList shelf;

static void tst_book_append(void) {
    puts("=====tst_book_append=====");
    LinkedListInit(&shelf);
    int code = 0;
    code += LinkedListAppend(&shelf, (Book){"01", "Practical Vim", 50.0f});
    code += LinkedListAppend(&shelf, (Book){"02", "C Primer Plus", 108.0f});
    code += LinkedListAppend(&shelf, (Book){"03", "C++ Primer Plus", 118.0f});
    code += LinkedListAppend(&shelf, (Book){"04", "笔记本", 18.8f});
    code += LinkedListAppend(&shelf, (Book){"05", "Signal and System", 98.0f});
    printf("append operation has a last sum status: %d.\n", code);
    puts("=========================\n");
}

static void itrprint(unsigned int i, NODE_TYPE v) {
    printf("<Book %s: %s -- %.2f>\n", v.bookid, v.bookname, v.price);
}

static void print_list(const char *listName, const LinkedList *list) {
    printf("LinkedList %s:\n", listName);
    LinkedListTraverse((LinkedList *)list, itrprint);
}

static void tst_book_iter(void) {
    puts("=====tst_book_iter=====");
    print_list("shelf", &shelf);
    puts("=======================\n");
}

static void tst_book_select(void) {
    puts("=====tst_book_select=====");
    Book buf;
    int code = LinkedListGet(&shelf, 2, &buf);
    printf("querying book #3 with status %d:\n", code);
    printf("Bookid: %s\nBookname: %s\nPrice: %.2f\n", buf.bookid, buf.bookname, buf.price);
    puts("=========================\n");
}

int main(void) {
    tst_book_append();
    tst_book_iter();
    tst_book_select();
    return 0;
}
