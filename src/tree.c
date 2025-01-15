#include "arraylist.h"
#include "rerror.h"
#include "tree.h"
#include "stack.h"
#include <stdbool.h>
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
    node->parent = NULL;
    node->balance = 0;
    node->height = 0;
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
            return -RERR_CORRUPTED;
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
    return -RERR_OK;
cleanup:
    TreeClear(tree);
    return -RERR_OOM;
}

static void avlins(unsigned i, NODE_TYPE v, void *buf) {
    (void)i;
    TreeAVLInsert((Tree *)buf, v);
}

int TreeConstruct(Tree *tree, const ArrayList *buf, enum ConstructorMethod m) {
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
    } else if (m == BALANCED_TREE) {
        tree->flags |= AVL_TREE;
        ArrayListTraverse((ArrayList *)buf, tree, avlins);
        return -RERR_OK;
    } else {
        OUT_OF_ENUM("Unknown method to construct tree");
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

// AVL tree

int TreeAVLFind(Tree *tree, NODE_TYPE v, unsigned int *cmpTimes) {
    if (!(tree->flags & AVL_TREE))
        return -RERR_NOTIMPLEMENTED;
    if (!tree->height)
        return -RERR_EMPTY;
    TreeNode *node = tree->root;
    int located = 0; // 1: found, 2: not found
    unsigned cmp_times = 0, index = 1;
    while (!located) {
        cmp_times++;
        if (v == node->data)
            located = 1;
        else if (v < node->data && node->lnode) {
            index <<= 1;
            node = node->lnode;
        } else if (v > node->data && node->rnode) {
            index = (index << 1) | 1;
            node = node->rnode;
        } else
            located = 2;
    }
    tree->cache = node;
    if (cmpTimes)
        *cmpTimes = cmp_times;
    if (located == 1)
        return index - 1;
    else
        return -RERR_NOTFOUND;
}

static TreeNode *balanceTree(TreeNode *node, Tree *tree);

int TreeAVLInsert(Tree *tree, NODE_TYPE v) {
    if (!(tree->flags & AVL_TREE))
        return -RERR_NOTIMPLEMENTED;
    int err = TreeAVLFind(tree, v, NULL); // using cache here
    if (err >= 0)
        return -RERR_EXISTED;
    assert(err == -RERR_NOTFOUND || err == -RERR_EMPTY);
    TreeNode *node = TreeNewNode(tree, v);
    if (!node)
        return -RERR_OOM;
    if (!tree->height) {
        tree->root = node;
        tree->height++;
        return -RERR_OK;
    }
    TreeNode *parent = tree->cache;
    node->parent = parent;
    if (v < parent->data)
        parent->lnode = node;
    else
        parent->rnode = node;

    while (parent) {
        if (parent->rnode == node)
            parent->balance--;
        else
            parent->balance++;
        if (parent->balance == 0)
            break; // subtree height left unchanged, stop updating parent's bf
        if (parent->balance == 2 || parent->balance == -2) {
            balanceTree(parent, tree);
            break; // there is no need to update parent's bf after rebalancing
        }
        // update from buttom to top
        parent->height++;
        node = parent;
        parent = parent->parent;
    }
    tree->height = tree->root->height;
    return -RERR_OK;
}

static TreeNode *deleteNode(TreeNode *node, TreeNode *recv, TreeNode ***slot, TreeNode **tofree) {
    if (!node->lnode && !node->rnode) {         // case 0
        // If parent's lnode/rnode is set to NULL, a problem may emerge when we detecting
        // whether node is parent's lnode or rnode as parent may have children both set
        // to NULL. In the result, we set it to a pointer on stack, so we can still trace
        // it later. When we consume the trace info, we set parent's lnode or rnode to NULL.
        if (node == node->parent->lnode)
            *slot = &node->parent->lnode;
        else
            *slot = &node->parent->rnode;
        **slot = recv;
        *recv = *node;
        return NULL;
    } else if (!node->lnode && node->rnode) {   // case 1
        if (node == node->parent->lnode)
            node->parent->lnode = node->rnode;
        else
            node->parent->rnode = node->rnode;
        node->rnode->parent = node->parent;
        return node->rnode;
    } else if (node->lnode && !node->rnode) {   // case 2
        if (node == node->parent->lnode)
            node->parent->lnode = node->lnode;
        else
            node->parent->rnode = node->lnode;
        node->lnode->parent = node->parent;
        return node->lnode;
    }
    // have 2 children                          // case 3
    TreeNode *successor = node->rnode;
    while (successor->lnode)
        successor = successor->lnode;
    NODE_TYPE t = node->data;
    node->data = successor->data;
    successor->data = t;
    // since we swap the data, the node to free also alters.
    *tofree = successor;
    return deleteNode(successor, recv, slot, tofree); // must be case 0 or 1
}

int TreeAVLDelete(Tree *tree, NODE_TYPE v) {
    if (!(tree->flags & AVL_TREE))
        return -RERR_NOTIMPLEMENTED;
    int err = TreeAVLFind(tree, v, NULL);
    if (err == -RERR_NOTFOUND)
        return -RERR_NOTFOUND;
    assert(err >= 0);
    TreeNode *node = tree->cache; // what we are seeking
    if (node == tree->root && (!node->lnode || !node->rnode)) {
        if (!node->lnode && !node->rnode) {
            tree->height = 0;
            tree->root = NULL;
        } else if (!node->lnode && node->rnode) {
            tree->root = node->rnode;
            tree->height = node->rnode->height + 1;
            node->rnode->parent = NULL;
        } else if (node->lnode && !node->rnode) {
            tree->root = node->lnode;
            tree->height = node->lnode->height + 1;
            node->lnode->parent = NULL;
        }
        free(node);
        tree->nodes--;
        return -RERR_OK;
        // if node has children, the real node we delete won't be root itself,
        // so we don't need to consider about these things here
    }
    TreeNode temp, **slot = NULL; // see notes in deleteNode
    TreeNode *change = deleteNode(node, &temp, &slot, &node);
    if (!change)
        change = &temp;
    free(node);
    tree->nodes--;
    TreeNode *parent = change->parent;
    node = change;

    while (parent) {
        if (parent->rnode == node)
            parent->balance++;
        else
            parent->balance--;
        if (slot) { // if a leaf is deleted, it must be processed immediately
            *slot = NULL; // as slot is used to know if node is lchild or rchild
            slot = NULL;  // now this information is useless
        }
        if (parent->balance == 1 || parent->balance == -1)
            break; // the subtree's height left unchanged
        if (parent->balance == 0)
            parent->height--;
        // update from buttom to top
        // when deleting a node, we may still need to update parent's bf
        if (parent->balance == 2 || parent->balance == -2)
            node = balanceTree(parent, tree); // new subtree's root
        else
            node = parent;
        if (!node)
            break; // in case after balanceTree, no need to update bf
        parent = node->parent;
    }
    tree->height = tree->root->height + 1;
    return -RERR_OK;
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define HEIGHT(node) ((node) ? (node)->height : -1)
/* returns new root of subtree */
static TreeNode *rotateLeft(TreeNode *node) {
    TreeNode *rnode = node->rnode;
    if (rnode->lnode)
        rnode->lnode->parent = node;
    rnode->parent = node->parent;
    node->parent = rnode;
    node->rnode = rnode->lnode;
    rnode->lnode = node;

    node->height = MAX(HEIGHT(node->lnode), HEIGHT(node->rnode)) + 1;
    node->balance = HEIGHT(node->lnode) - HEIGHT(node->rnode);
    rnode->height = MAX(HEIGHT(rnode->lnode), HEIGHT(rnode->rnode)) + 1;
    rnode->balance = HEIGHT(rnode->lnode) - HEIGHT(rnode->rnode);
    return rnode;
}

/* returns new root of subtree */
static TreeNode *rotateRight(TreeNode *node) {
    TreeNode *lnode = node->lnode;
    if (lnode->rnode)
        lnode->rnode->parent = node;
    lnode->parent = node->parent;
    node->parent = lnode;
    node->lnode = lnode->rnode;
    lnode->rnode = node;

    node->height = MAX(HEIGHT(node->lnode), HEIGHT(node->rnode)) + 1;
    node->balance = HEIGHT(node->lnode) - HEIGHT(node->rnode);
    lnode->height = MAX(HEIGHT(lnode->lnode), HEIGHT(lnode->rnode)) + 1;
    lnode->balance = HEIGHT(lnode->lnode) - HEIGHT(lnode->rnode);
    return lnode;
}

static TreeNode *balanceTree(TreeNode *node, Tree *tree) {
    int oldHeight = node->height;
    TreeNode **slot, *child;
    // to update parent's child
    if (!node->parent) // root
        slot = &tree->root;
    else if (node->parent->lnode == node)
        slot = &node->parent->lnode;
    else
        slot = &node->parent->rnode;

    if (node->balance == 2) {
        child = node->lnode;
        if (child->balance == 1) {          // LL
            node = rotateRight(node);
            *slot = node;
        } else if (child->balance == -1) {  // LR
            child = rotateLeft(child);
            node->lnode = child;
            node = rotateRight(node);
            *slot = node;
        } else
            OUT_OF_ENUM("Not LL or LR");
    } else if (node->balance == -2) {
        child = node->rnode;
        if (child->balance == -1) {         // RR
            node = rotateLeft(node);
            *slot = node;
        } else if (child->balance == 1) {   // RL
            child = rotateRight(child);
            node->rnode = child;
            node = rotateLeft(node);
            *slot = node;
        } else
            OUT_OF_ENUM("Not RR or RL");
    } else {
        OUT_OF_ENUM("|balance| is not 2");
    }
    if (oldHeight != node->height)
        return node;
    else // if height left unchanged, no need to update parent's bf
        return NULL;
}
