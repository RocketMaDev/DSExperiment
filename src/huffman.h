/**
 * Chapter 5 - TREE
 * Experiment 2.3 - Huffman Encoding
 */
#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__
#include "tree.h"
#include "arraylist.h"

typedef struct {
    unsigned short bits;
    unsigned short nbit;
} HuffmanCode;

typedef struct {
    Tree tree;
    const ArrayList *weights;
    HuffmanCode *codes;
    TreeNode **nodes;
} HuffmanTree;

#define CONCAT(weight, index) (((weight) << 8) | (index))
#define WEIGHT(var) (((unsigned NODE_TYPE)(var)) >> 8)
#define INDEX(var) ((var) & 0xff)

/**
 * Initialize an empty huffman tree.
 *
 * @param tree  An uninitialized huffman tree.
 */
void HuffmanTreeInit(HuffmanTree *tree);
/**
 * Release the huffman tree. Destructor.
 *
 * @param tree The tree to Release.
 */
void HuffmanTreeRelease(HuffmanTree *tree);
/**
 * Construct a huffman tree with a list of weights. The tree
 * should have Initialized.
 *
 * @param tree      An empty huffman tree.
 * @param weights   A list contains nodes' weights.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_OOM      Insufficient memory when running.
 * @exception RERR_EMPTY    `weights` is empty.
 */
int HuffmanTreeConstruct(HuffmanTree *tree, const ArrayList *weights);
/**
 * Fetch the huffman code from huffman tree. Should run `HuffmanTreeConstruct`
 * first.
 *
 * @param tree  A huffman tree that has calculated huffman code.
 * @param pcode A pointer to store a list of `HuffmanCode`.
 * @param size  To store the size of `HuffmanCode` array.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_EMPTY    If huffman code is not calculated yet.
 */
int HuffmanTreeCode(const HuffmanTree *tree, HuffmanCode **pcode, unsigned int *size);
#endif
