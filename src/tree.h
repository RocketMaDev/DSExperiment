/**
 * Chapter 5 - TREE
 * Experiment 2.1 - Binary Tree
 * Experiment 2.2 - Threading Tree
 *
 * Chapter 7 - SEARCHING
 * Experiment 4.2 - Dynamic Searching
 */
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
    // only works in AVL_TREE
    struct __tnode *parent;
    unsigned char height;
    signed char balance;
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
    TreeNode *cache; // only works in AVL_TREE
} Tree;

enum TreeFlag {
    PLAIN_TREE = 0,
    THREADING_TREE = 1,
    HUFFMAN_TREE = 2,
    AVL_TREE = 4,
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
    /* A.K.A. AVL tree */
    BALANCED_TREE,
};

/**
 * Initialize a tree.
 *
 * @param tree  An uninitialized tree.
 */
void TreeInit(Tree *tree);
/**
 * Remove all nodes in the tree.
 *
 * @param tree  A tree.
 */
void TreeClear(Tree *tree);
/**
 * Release a tree completely. Equivalent to `TreeClear`.
 *
 * @param tree  A tree.
 */
void TreeRelease(Tree *tree);

/**
 * Provide a new node with element `v`, and increase node count
 * in tree.
 *
 * @param tree  Which tree to add node.
 * @param v     The element to add.
 * @return A chunk of memory if successfully allocated, or `NULL` if failed.
 */
TreeNode *TreeNewNode(Tree *tree, NODE_TYPE v);
/* Just free it. */
void TreeFreeNode(TreeNode *);
/**
 * Construct a tree with a list of elements.
 *
 * @param tree  An empty, initialized tree.
 * @param list  A list of elements.
 * @param type  One of constructor methods.
 * @return A `-status` code where `status` is one of member in `RERROR`. `0` if no error.
 *
 * @exception RERR_EMPTY    If list is empty.
 * @exception RERR_CORRUPTED - `LINEAR`: list is not a valid BFS sequence.
 *                           - `BINARY_SEARCH`: duplicate elements in list.
 * @exception RERR_EXISTED  If duplicate elements in list.
 * @exception RERR_OOM      Insufficient memory when constructing.
 */
int TreeConstruct(Tree *tree, const ArrayList *list, enum ConstructorMethod type);
/**
 * Traverse tree nodes with `func`.
 *
 * @param tree  The tree to traverse.
 * @param order The order to traverse a tree.
 * @param buf   An arbitrary pointer which will be `func`'s third arg when called.
 * @param func  The func to traverse.
 *
 * @note THREADING_TREE only support in-order.
 */
void TreeTraverse(const Tree *tree, enum TraverseOrder order, void *buf, void (*func)(const TreeNode *, void *buf));
/**
 * Calculate count of specified degree in the tree.
 *
 * @param tree      The tree to calc.
 * @param degree    Which degree to calc. (0, 1, 2)
 * @return The count of specified degree.
 */
unsigned int TreeDegreeCount(const Tree *tree, unsigned int degree);
/**
 * Get the height of the tree.
 *
 * @param tree  The tree to work on.
 * @return The height. For huffman tree, always return 0.
 */
unsigned int TreeHeight(const Tree *tree);

/**
 * Insert a node into a AVL tree.
 *
 * @param tree  The tree to work on.
 * @param v     The element to insert.
 * @return A `-status` code where `status` is one of member in `RERROR`. `0` if no error.
 *
 * @exception RERR_NOTIMPLEMENTED   If tree is not a avl tree.
 * @exception RERR_EXISTED          The element is existed in tree.
 * @exception RERR_OOM              Insufficient memory when running.
 */
int TreeAVLInsert(Tree *tree, NODE_TYPE v);
/**
 * Delete a node the same as `v` from a AVL tree.
 *
 * @param tree  The tree to work on.
 * @param v     The element to search in the tree.
 * @return A `-status` code where `status` is one of member in `RERROR`. `0` if no error.
 *
 * @exception RERR_NOTIMPLEMENTED   If tree is not a avl tree.
 * @exception RERR_NOTFOUND         The element is not found in tree.
 */
int TreeAVLDelete(Tree *tree, NODE_TYPE v);
/**
 * Find a node same as `v` in the AVL tree.
 *
 * @param tree      The tree to work on.
 * @param v         The element to search in the tree.
 * @param cmpTimes  If not `NULL`, store comparison times.
 * @return The BFS order of node in tree if found. Or else `-status`.
 *
 * @exception RERR_NOTIMPLEMENTED   If tree is not a avl tree.
 * @exception RERR_EMPTY            If the tree is empty.
 * @exception RERR_NOTFOUND         If element is not found in tree.
 */
int TreeAVLFind(Tree *tree, NODE_TYPE v, Nullable unsigned *cmpTimes);
#endif
