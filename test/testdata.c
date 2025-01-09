#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include "../src/arraylist.h"
typedef struct {
    unsigned key;
    unsigned age;
    char name[0xc];
    bool female;
} Record;

static Record infos[] = {
    {56, 19, "Zhang", true},
    {19, 20, "Wang", true},
    {80, 19, "Zhou", true},
    {5, 20, "Huang", false},
    {21, 20, "Zheng", false},
    {64, 19, "Li", false},
    {89, 18, "Liu", true},
    {13, 19, "Qian", true},
    {38, 20, "Sun", false},
    {75, 20, "Zhao", false},
    {92, 20, "Chen", false},
};
#define ninfo (sizeof(infos) / sizeof(Record))
// 56,19,80,5,21,64,89,13,38,75,92

static Record *map[32];
#define MAP(k) map[(k) & 0x1f]
static unsigned keys[ninfo];

__attribute__((constructor))
static void map_init(void) {
    for (unsigned i = 0; i < ninfo; i++) {
        MAP(infos[i].key) = infos + i;
        keys[i] = infos[i].key;
    }
}

ArrayList tmp = {(int *)keys, ninfo, ninfo};

static void pitem(unsigned i, int v, void *buf) {
    (void)i, (void)buf;
    printf("|%3d|%6s|%6s|%3d|\n", v, MAP(v)->name, MAP(v)->female ? "Female" : "Male", MAP(v)->age);
}

extern ArrayList *list;

static void tst_create_list(void) {
    puts("=====tst_create_list=====");
    ArrayListInit(list, ninfo);
    int err = ArrayListExtend(list, &tmp);
    printf("Created search list with status %d\n", err);
    puts("Info table: [");
    printf("|%3s|%6s|%6s|%3s|\n", "key", "name", "sex", "age");
    ArrayListTraverse(list, NULL, pitem);
    puts("]");
    puts("=========================\n");
}

void PrintArrayList(const NODE_TYPE *arr, unsigned size, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf(": [");
    for (unsigned i = 0; i < size - 1; i++)
        printf("%d, ", arr[i]);
    printf("%d]\n", arr[size - 1]);
}
