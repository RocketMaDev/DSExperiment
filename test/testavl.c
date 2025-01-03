#include "../src/tree.h"
#include "testdata.c"
#include <stdio.h>
#include <assert.h>

static Tree x, *tree = &x;

static void pelem(const TreeNode *node, void *buf) {
    (void)buf;
    printf("%d ", node->data);
}

#define ptree(msg) \
    printf(msg); \
    TreeTraverse(tree, IN_ORDER, NULL, pelem); \
    putchar('\n')

static void tst_create_avl(void) {
    puts("=====tst_create_avl=====");
    ArrayList list = {(int *)keys, ninfo, ninfo};
    TreeInit(tree);
    TreeConstruct(tree, &list, BALANCED_TREE);
    ptree("Created AVL tree: ");
    puts("========================\n");
}

static void tst_find_avl(void) {
    puts("=====tst_find_avl=====");
    unsigned sum = 0;
    unsigned cur;
    for (unsigned i = 0; i < ninfo; i++) {
        int err = TreeAVLFind(tree, keys[i], &cur);
        printf("Found %d @ %d(BFS order) with %d times of comparison\n", keys[i], err, cur);
        sum += cur;
    }
    int size = ninfo;
    printf("Successful AVL: %.2lf\n", sum / (double)size);
    int fail[] = {4, 57, 20, 81, 6, 22, 65, 90, 14, 39, 76, 93};
    sum = 0;
    for (unsigned i = 0; i <= ninfo; i++) {
        int err = TreeAVLFind(tree, fail[i], &cur);
        assert(err == -RERR_NOTFOUND);
        printf("%d not found with %d times of comparison\n", fail[i], cur);
        sum += cur;
    }
    printf("Failed AVL: %.2lf\n", sum / ((double)size + 1));
    puts("======================\n");
}

static void tst_insert_avl(void) {
    puts("=====tst_insert_avl=====");
    int values[] = {82, 60, 19, 39, 58, 62, 57, 25};
    register unsigned size = sizeof(values) / sizeof(int);
    for (unsigned i = 0; i < size; i++) {
        int err = TreeAVLInsert(tree, values[i]);
        assert(err == -RERR_OK || err == -RERR_EXISTED);
        if (err == 0)
            printf("Insert %d successfully\n", values[i]);
        else
            printf("Insert existing %d failed\n", values[i]);
    }
    ptree("AVL tree after 4 insertions: ");
    puts("========================\n");
}

static void tst_remove_avl(void) {
    puts("=====tst_remove_avl=====");
    int values[] = {64, 5, 7, 13, 56};
    register unsigned size = sizeof(values) / sizeof(int);
    for (unsigned i = 0; i < size; i++) {
        int err = TreeAVLDelete(tree, values[i]);
        if (err == 0)
            printf("Remove %d successfully\n", values[i]);
        else
            printf("Remove existing %d failed\n", values[i]);
    }
    ptree("AVL tree after 4 removals: ");
    puts("========================\n");
}

int main(void) {
    tst_create_avl();
    tst_find_avl();
    tst_insert_avl();
    tst_remove_avl();
    TreeRelease(tree);
    return 0;
}
