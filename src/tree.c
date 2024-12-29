#include "arraylist.h"
#include "rerror.h"
#include "tree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void TreeInit(Tree *tree) {
    tree->root = NULL;
    tree->height = 0;
    tree->flags = PLAIN_TREE;
    tree->nodes = 0;
}

TreeNode *TreeNewNode(Tree *tree, NODE_TYPE v) {
    register TreeNode *node = malloc(sizeof(TreeNode));
    if (!node)
        return NULL;
    tree->nodes++;
    node->lnode = NULL;
    node->rnode = NULL;
    node->data = v;
    node->flags &= PLAIN_NODE;
    return node;
}

void TreeFreeNode(TreeNode *node) {
    free(node);
}

typedef void (*fp)(const TreeNode *v, void *buf);
static void pre_traverse(const TreeNode *node, fp func, void *buf) {
    func(node, buf);
    if (node->lnode)
        pre_traverse(node->lnode, func, buf);
    if (node->rnode)
        pre_traverse(node->rnode, func, buf);
}
static void in_traverse(const TreeNode *node, fp func, void *buf) {
    if (node->lnode)
        in_traverse(node->lnode, func, buf);
    func(node, buf);
    if (node->rnode)
        in_traverse(node->rnode, func, buf);
}
static void post_traverse(const TreeNode *node, fp func, void *buf) {
    if (node->lnode)
        post_traverse(node->lnode, func, buf);
    if (node->rnode)
        post_traverse(node->rnode, func, buf);
    func(node, buf);
}

void TreeTraverse(const Tree *tree, enum TraverseOrder order, void *buf, fp func) {
    if (tree->height == 0)
        return;
    if (tree->flags & THREADING_TREE) {
        if (order == IN_ORDER) {
            TreeNode *node = tree->root;
            while (node) {
                while ((node->flags & LTHREAD) == 0)
                    node = node->lnode;
                func(node, buf);
                while (node->flags & RTHREAD && node->rnode) {
                    node = node->rnode;
                    func(node, buf);
                }
                node = node->rnode;
            }
        } else {
            OUT_OF_ENUM("PRE_ORDER & POST_ORDER not implemented for threading_tree");
        }
    } else if (order == PRE_ORDER)
        pre_traverse(tree->root, func, buf);
    else if (order == IN_ORDER)
        in_traverse(tree->root, func, buf);
    else if (order == POST_ORDER)
        post_traverse(tree->root, func, buf);
    else
        OUT_OF_ENUM("Unknown order");
}

struct __counter {
    unsigned int degree;
    unsigned int count;
};

static void traverse(const TreeNode *node, void *buf) {
    unsigned int available = 0;
    if (node->lnode)
        available++;
    if (node->rnode)
        available++;
    if (available == ((struct __counter *)buf)->degree)
        ((struct __counter *)buf)->count++;
}

int TreeDegreeCount(const Tree *tree, unsigned int degree) {
    if (tree->height == 0)
        return -RERR_EMPTY;
    struct __counter cnt = {degree, 0};
    TreeTraverse(tree, PRE_ORDER, &cnt, traverse);
    return cnt.count;
}

unsigned int TreeHeight(const Tree *tree) {
    if (tree->flags & HUFFMAN_TREE)
        return 0;   // not implemented
    return tree->height;
}

#define IFOOM(tree, v) \
    if (!(node = TreeNewNode(tree, v))) \
        goto cleanup
__attribute__((always_inline))
static inline int constructBYplain(Tree *tree, const ArrayList *list) {
    NODE_TYPE tmp;
    // test validity
#ifdef VALIDATE_OVERRIDE
    if (list->size == 0 || (ArrayListGet(list, 0, &tmp), !validate(&tmp)))
#else
    if (list->size == 0 || (ArrayListGet(list, 0, &tmp), tmp < 0))
#endif
        return -RERR_EMPTY;
    // assuming int
    NODE_TYPE *arr = list->arr;
    unsigned int size = list->size;
    for (unsigned int i = 1; i < size; i++)
#ifdef VALIDATE_OVERRIDE
        if (validate(arr + i) && !validate(arr + ((i + 1) >> 1) - 1))
#else
        if (arr[i] >= 0 && arr[((i + 1) >> 1) - 1] < 0) // idx 3 depends on idx 1
#endif
            return -RERR_CORRUPPTED;
    ArrayList stack;
    int err = StackInit(&stack);
    if (err)
        goto cleanup;

    TreeNode *node, *ptn;
    IFOOM(tree, arr[0]); // root must exist
    tree->root = node;
    tree->height = 1;
    for (unsigned int i = 1; i < list->size; i++) {
#ifdef VALIDATE_OVERRIDE
        if (!validate(arr + i))
#else
        if (arr[i] < 0)
#endif
            continue;
        ptn = tree->root;
        register unsigned int idx = i;
        // identify route
        while (idx) {
            // mark left child by 0
            StackPush(&stack, idx & 1 ? 0 : 1);
            // goto parent idx
            idx = ((idx + 1) >> 1) - 1;
        }
        // update height of tree now
        if (stack.size + 1 > tree->height)
            tree->height = stack.size + 1;
        // get to node
        while (stack.size > 1) {
            StackPop(&stack, &tmp);
            if (tmp)
                ptn = ptn->rnode;
            else
                ptn = ptn->lnode;
        }
#ifdef VALIDATE_OVERRIDE
        IFOOM(fetch_real(arr + 1));
#else
        IFOOM(tree, ((arr[i]) << 1) >> 1); // sign extend 1 bit
#endif
        assert(StackPop(&stack, &tmp) >= 0);
        if (tmp)
            ptn->rnode = node;
        else
            ptn->lnode = node;
        assert(stack.size == 0);
    }
    ArrayListRelease(&stack);
    return -RERR_OK;
cleanup:
    ArrayListRelease(&stack);
    TreeClear(tree);
    return -RERR_OOM;
}

static void threading_tree(TreeNode *node, TreeNode **prev) {
    if (!node)
        return;
    threading_tree(node->lnode, prev);
    if (!node->lnode) {
        node->flags |= LTHREAD;
        node->lnode = *prev;
    }
    if (*prev && !(*prev)->rnode) {
        (*prev)->flags |= RTHREAD;
        (*prev)->rnode = node;
    }
    *prev = node;
    threading_tree(node->rnode, prev);
}

#ifndef VALIDATE_OVERRIDE
static inline int compare(NODE_TYPE *a, NODE_TYPE *b) {
    return *a - *b;
}
#endif
__attribute__((always_inline))
static inline int constructBYbs(Tree *tree, const ArrayList *list) {
    if (list->size == 0)
        return -RERR_EMPTY;
    TreeNode *node;
    NODE_TYPE *arr = list->arr;
    unsigned int size = list->size;
    // assuming no duplication
    IFOOM(tree, arr[0]);
    tree->root = node;
    tree->height = 1;
    for (unsigned int i = 1; i < size; i++) {
        TreeNode *ptn = tree->root;
        unsigned int layer = 1;
        node = NULL;
        while (!node) { // exit when node is inserted
            layer++;
            register int r;
            if ((r = compare(arr + i, &ptn->data)) < 0) {
                if (ptn->lnode) {
                    ptn = ptn->lnode;
                    continue;
                }
                IFOOM(tree, arr[i]);
                ptn->lnode = node;
            } else if (r > 0) {
                if (ptn->rnode) {
                    ptn = ptn->rnode;
                    continue;
                }
                IFOOM(tree, arr[i]);
                ptn->rnode = node;
            } else {
                assert(false); // duplicated value detected
            }
            if (layer > tree->height)
                tree->height = layer;
        }
    }
    tree->flags |= BINSEC_TREE;
    return -RERR_OK;
cleanup:
    TreeClear(tree);
    return -RERR_OOM;
}

int TreeConstruct(Tree *tree, const void *buf, enum ConstructorMethod m) {
    if (m == BINARY_SEARCH)
        return constructBYbs(tree, buf);
    else if (m == LINEAR)
        return constructBYplain(tree, buf);
    else if (m == LINEAR_WITH_THREAD) {
        tree->flags |= THREADING_TREE;
        int err = constructBYplain(tree, buf);
        if (err)
            return err;
        TreeNode *prev = NULL;
        threading_tree(tree->root, &prev);
        prev->flags = RTHREAD; // Rightmost node
        return -RERR_OK;
    } else {
        fprintf(stderr, "Unknown constructor method @%s#L%d, exit\n", __FILE__, __LINE__);
        exit(1);
    }
}

void tree_free(const TreeNode *node, void *buf) {
    (void)buf; // suppress unused warning
    free((void *)node);
}

struct __node_list {
    TreeNode **list;
    unsigned int idx;
};

void thread_tree_free(const TreeNode *node, void *buf) {
    register struct __node_list *nodes = (struct __node_list *)buf;
    nodes->list[nodes->idx++] = (TreeNode *)node;
}

void TreeClear(Tree *tree) {
    if (!tree->height)
        return;
    if (tree->flags & THREADING_TREE) {
        struct __node_list nodes;
        nodes.list = malloc(sizeof(TreeNode *) * tree->nodes);
        if (!nodes.list)
            OUT_OF_ENUM("No memory to free tree...");
        nodes.idx = 0;
        TreeTraverse(tree, IN_ORDER, &nodes, thread_tree_free);
        for (unsigned int i = 0; i < tree->nodes; i++)
            free(nodes.list[i]);
        free(nodes.list);
    } else
        // after free, we lose pointer of child, so we need to free last
        post_traverse(tree->root, tree_free, NULL);
    tree->root = NULL;
    tree->height = 0;
}

void TreeRelease(Tree *tree) {
    TreeClear(tree);
}
