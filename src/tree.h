#ifndef __TREE_H__
#define __TREE_H__
#ifndef NODE_TYPE
#   define NODE_TYPE int
#endif
#include "arraylist.h"
#include <stdbool.h>
#ifdef VALIDATE_OVERRIDE
extern bool validate(NODE_TYPE *v);
extern NODE_TYPE fetch_real(NODE_TYPE *v);
extern int compare(NODE_TYPE *a, NODE_TYPE *b);
#endif
typedef struct __tnode {
    struct __tnode *lnode;
    struct __tnode *rnode;
    NODE_TYPE data;
    unsigned int flags;
} TreeNode;

enum NodeFlags {
    PLAIN_NODE = 0,
    LTHREAD = 1,
    RTHREAD = 2,
};

typedef struct {
    TreeNode *root;
    unsigned int height;
    unsigned int flags;
    unsigned int nodes;
} Tree;

enum TreeFlag {
    PLAIN_TREE = 0,
    THREADING_TREE = 1,
    HUFFMAN_TREE = 2,
    BINSEC_TREE = 4,
};

enum TraverseOrder {
    PRE_ORDER,
    IN_ORDER,
    POST_ORDER,
};

enum ConstructorMethod {
    /* all values are considered valid */
    BINARY_SEARCH,
    /* the highest bit is used as not-valid flag */
    LINEAR,
    /* the highest bit is used as not-valid flag, add support of threading */
    LINEAR_WITH_THREAD,
};

void TreeInit(Tree *);
void TreeClear(Tree *);
void TreeRelease(Tree *);
TreeNode *TreeNewNode(Tree *tree, NODE_TYPE v);
void TreeFreeNode(TreeNode *);
/* buf is ArrayList * */
int TreeConstruct(Tree *tree, const void *buf, enum ConstructorMethod);
void TreeTraverse(const Tree *tree, enum TraverseOrder order, void *buf, void (*func)(const TreeNode *, void *buf));
int TreeDegreeCount(const Tree *tree, unsigned int degree);
unsigned int TreeHeight(const Tree *tree);

int BSTInsert(Tree *tree, NODE_TYPE v);
int BSTDelete(Tree *tree, NODE_TYPE v);
int BSTFind(Tree *tree, NODE_TYPE v, Nullable unsigned *cmpTimes);
#endif
