#ifndef __TREE_H__
#define __TREE_H__
#ifndef NODE_TYPE
#   define NODE_TYPE int
#endif
#include <stdbool.h>
#ifdef VALIDATE_OVERRIDE
extern bool validate(NODE_TYPE *v);
extern NODE_TYPE fetch_real(NODE_TYPE *v);
extern int compare(NODE_TYPE *a, NODE_TYPE *b);
#endif
typedef struct __tnode {
    NODE_TYPE data;
    struct __tnode *lnode;
    struct __tnode *rnode;
} TreeNode;

typedef struct {
    TreeNode *root;
    unsigned int height;
} Tree;

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
};

void TreeInit(Tree *);
void TreeClear(Tree *);
void TreeRelease(Tree *);
TreeNode *TreeNewNode(NODE_TYPE v);
void TreeFreeNode(TreeNode *);
int TreeConstruct(Tree *tree, const void *buf, enum ConstructorMethod);
void TreeTraverse(const Tree *tree, enum TraverseOrder order, void *buf, void (*func)(const TreeNode *, void *buf));
int TreeDegreeCount(const Tree *tree, unsigned int degree);
unsigned int TreeHeight(const Tree *tree);

#endif
