#include "../src/tree.h"
#include "../src/arraylist.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

static Tree plainTree;
static void tst_linear(void) {
    puts("=====tst_linear=====");
    TreeInit(&plainTree);
    int plain[] = {'A', 'B', 'C', 'D', -1, 'E', -1, -1, 'F'};
    ArrayList plainList = {plain, sizeof(plain) / sizeof(int), sizeof(plain) / sizeof(int)};
    int err = TreeConstruct(&plainTree, &plainList, LINEAR);
    printf("Constructed plainTree with status %d\n", err);
    puts("====================\n");
}

static void pnode(const TreeNode *node, void *buf) {
    (void)buf; // suppress unused warning
    putchar(node->data);
}

static void tst_traverse(void) {
    puts("=====tst_traverse=====");
    printf("Traverse by preorder: ");
    TreeTraverse(&plainTree, PRE_ORDER, NULL, pnode);
    putchar('\n');
    printf("Traverse by inorder: ");
    TreeTraverse(&plainTree, IN_ORDER, NULL, pnode);
    putchar('\n');
    printf("Traverse by postorder: ");
    TreeTraverse(&plainTree, POST_ORDER, NULL, pnode);
    putchar('\n');
    puts("======================\n");
}

static void tst_degree_count(void) {
    puts("=====tst_degree_count=====");
    printf("plainTree has %d, %d, %d nodes for 0, 1, 2 degrees\n",
            TreeDegreeCount(&plainTree, 0),
            TreeDegreeCount(&plainTree, 1),
            TreeDegreeCount(&plainTree, 2));
    puts("==========================\n");
}

#ifdef DEBUG
static void printTree(const Tree *);
static void printSubtree(const TreeNode *, const char *);
#endif

static void tst_bst(void) {
    puts("=====tst_bst=====");
    Tree tree1, tree2;
    TreeInit(&tree1);
    TreeInit(&tree2);
    int arr1[] = {'D', 'B', 'F', 'C', 'A', 'E', 'G'};
    int arr2[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    ArrayList list1 = {arr1, sizeof(arr1) / sizeof(int), sizeof(arr1) / sizeof(int)};
    ArrayList list2 = {arr2, sizeof(arr2) / sizeof(int), sizeof(arr2) / sizeof(int)};
    TreeConstruct(&tree1, &list1, BINARY_SEARCH);
    TreeConstruct(&tree2, &list2, BINARY_SEARCH);

#ifdef DEBUG
    puts("Tree1:");
    printTree(&tree1);
    puts("Tree2:");
    printTree(&tree2);
#endif

    printf("Traversing \"DBFCAEG\": ");
    TreeTraverse(&tree1, IN_ORDER, NULL, pnode);
    putchar('\n');
    printf("Traversing \"ABCDEFG\": ");
    TreeTraverse(&tree2, IN_ORDER, NULL, pnode);
    putchar('\n');
    printf("\"DBFCAEG\"'s height is %d while \"ABCDEFG\"'s height is %d\n",
            TreeHeight(&tree1), TreeHeight(&tree2));
    TreeRelease(&tree1);
    TreeRelease(&tree2);
    puts("=================\n");
}

#ifdef DEBUG
static void printTree(const Tree* tree) {
    if (tree->root == NULL) {
        return;
    }
    putchar(tree->root->data);
    putchar('\n');
    printSubtree(tree->root, "");
    putchar('\n');
}

static void printSubtree(const TreeNode* root, const char *prefix) {
    if (root == NULL) {
        return;
    }
    bool hasLeft = (root->lnode != NULL);
    bool hasRight = (root->rnode != NULL);
    if (!hasLeft && !hasRight) {
        return;
    }

    printf("%s%s%s", prefix,
            ((hasLeft  && hasRight) ? "├── " : ""),
            ((!hasLeft && hasRight) ? "└── " : ""));

#define MAX_CHARS 63
    char newPrefix[MAX_CHARS + 1]; // may be insufficient
    newPrefix[MAX_CHARS] = '\0';   // clear out last char in case all MAX_CHARS are written

    if (hasRight) {
        bool printStrand = (hasLeft && hasRight && (root->rnode->rnode != NULL || root->rnode->lnode != NULL));
        strncpy(newPrefix, prefix, MAX_CHARS);
        strncat(newPrefix, (printStrand ? "│   " : "    "), MAX_CHARS - strlen(prefix) - 4);
        putchar(root->rnode->data);
        putchar('\n');
        printSubtree(root->rnode, newPrefix);
    }

    if (hasLeft) {
        printf("%s%s%c\n", (hasRight ? prefix : ""), "└── ", root->lnode->data);
        strncpy(newPrefix, prefix, MAX_CHARS);
        strncat(newPrefix,  "    ", MAX_CHARS - strlen(prefix) - 4);
        printSubtree(root->lnode, newPrefix);
    }
}
#endif

static Tree threadTree;
static void tst_thread(void) {
    puts("=====tst_thread=====");
    TreeInit(&threadTree);
    int plain[] = {'A', 'B', 'C', 'D', -1, 'E', -1, -1, 'F'};
    ArrayList plainList = {plain, sizeof(plain) / sizeof(int), sizeof(plain) / sizeof(int)};
    int err = TreeConstruct(&threadTree, &plainList, LINEAR_WITH_THREAD);
    printf("Constructed threadTree with status %d\n", err);
    puts("====================\n");
}

static void print_node(const TreeNode *node, void *buf) {
    (void)buf;
    printf("|%d|%c|%c|%c|%d|\n", ((node->flags & LTHREAD) == LTHREAD),
            node->lnode ? node->lnode->data : '^',
            node->data,
            node->rnode ? node->rnode->data : '^',
            ((node->flags & RTHREAD) == RTHREAD));
}

static void tst_thread_traverse(void) {
    puts("=====tst_thread_traverse=====");
    puts("Traverse by inorder:");
    TreeTraverse(&threadTree, IN_ORDER, NULL, print_node);
    puts("=============================\n");
}

int main(void) {
    tst_linear();
    tst_traverse();
    tst_degree_count();
    tst_bst();
    tst_thread();
    tst_thread_traverse();
    TreeRelease(&plainTree);
    TreeRelease(&threadTree);
    return 0;
}
