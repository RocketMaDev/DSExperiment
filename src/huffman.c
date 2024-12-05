#include "huffman.h"
#include "rerror.h"
#include "tree.h"
#include "arraylist.h"
#include "stack.h"
#include <stdlib.h>
#include <stddef.h>
#include <sys/cdefs.h>

void HuffmanTreeInit(HuffmanTree *tree) {
    TreeInit(&tree->tree);
    tree->codes = NULL;
    tree->weights = NULL;
    tree->tree.flags |= HUFFMAN_TREE;
}

void HuffmanTreeRelease(HuffmanTree *tree) {
    if (!tree->codes)
        return;
    TreeRelease(&tree->tree);
    free(tree->codes);
    tree->weights = NULL;
}

int HuffmanTreeCode(const HuffmanTree *tree, HuffmanCode **pcode, unsigned int *size) {
    if (!tree->codes)
        return -RERR_EMPTY;
    *pcode = tree->codes;
    *size = tree->weights->size;
    return -RERR_OK;
}

static int cmp(const void *a, const void *b) {
    return WEIGHT(*(const NODE_TYPE *)b) - WEIGHT(*(const NODE_TYPE *)a);
}

__attribute__((always_inline))
static inline int process_easycases(HuffmanTree *tree, const ArrayList *weights) {
    if (weights->size == 0)
        return -RERR_EMPTY;
    tree->codes = malloc(sizeof(HuffmanCode));
    if (!tree->codes)
        return -RERR_OOM;
    tree->codes[0] = (HuffmanCode){0, 1};
    NODE_TYPE t;
    ArrayListGet(weights, 0, &t);
    tree->tree.root = TreeNewNode(&tree->tree, CONCAT(t, 0));
    tree->tree.height = 1;
    if (!tree->tree.root)
        return -RERR_OOM;
    return -RERR_OK;
}

static void traverse_encode(TreeNode *node, HuffmanCode *clist, unsigned int indexRange, HuffmanCode code);

int HuffmanTreeConstruct(HuffmanTree *tree, const ArrayList *weights) {
    // Construct Huffman tree 
    tree->weights = weights;
    if (weights->size < 2)
        return process_easycases(tree, weights);
    ArrayList concats;
    ArrayListInit(&concats, weights->size);
    ArrayListExtend(&concats, weights);
    NODE_TYPE *list = concats.arr;
    HuffmanCode *clist = NULL;
    TreeNode **nodes = malloc(sizeof(TreeNode *) * concats.size * 2);
    if (!nodes)
        goto cleanup;
    unsigned int idx = 0;
    for (unsigned int i = 0; i < concats.size; i++) {
        list[i] = CONCAT(list[i], i);
        TreeNode *node = TreeNewNode(&tree->tree, list[i]);
        if (!node)
            goto cleanup;
        nodes[idx++] = node;
    }
    while (concats.size > 1) {
        qsort(list, concats.size, sizeof(NODE_TYPE), cmp); // put smallest at back
        NODE_TYPE n1, n2;
        StackPop(&concats, &n1);
        StackPop(&concats, &n2);
        TreeNode *tonew = TreeNewNode(&tree->tree, CONCAT(WEIGHT(n1) + WEIGHT(n2), idx));
        if (!tonew)
            goto cleanup;
        tonew->lnode = nodes[INDEX(n1)];
        tonew->rnode = nodes[INDEX(n2)];
        nodes[idx++] = tonew;
        StackPush(&concats, tonew->data);
    }
    tree->tree.root = nodes[INDEX(list[0])]; // the remaining one will be root
    ArrayListRelease(&concats);

    // Generate Huffman encoding
    clist = malloc(sizeof(HuffmanCode) * weights->size);
    if (!clist)
        goto cleanup;
    traverse_encode(tree->tree.root, clist, weights->size, (HuffmanCode){0, 0});
    tree->tree.height = 1; // needed to release the tree
    tree->codes = clist;
    free(nodes);
    return -RERR_OK;

cleanup:
    free(nodes);
    return -RERR_OOM;
}

static void traverse_encode(TreeNode *node, HuffmanCode *clist, unsigned int indexRange,
        HuffmanCode code) {
    if (node->lnode)
        traverse_encode(node->lnode, clist, indexRange, 
                (HuffmanCode){code.bits, code.nbit + 1});
    if (node->rnode)
        traverse_encode(node->rnode, clist, indexRange, 
                (HuffmanCode){code.bits | (1 << code.nbit), code.nbit + 1});
    if (INDEX(node->data) < indexRange)
        clist[INDEX(node->data)] = code;
}
