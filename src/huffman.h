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
} HuffmanTree;

#define CONCAT(weight, index) (((weight) << 8) | (index))
#define WEIGHT(var) (((unsigned NODE_TYPE)(var)) >> 8)
#define INDEX(var) ((var) & 0xff)

void HuffmanTreeInit(HuffmanTree *tree);
void HuffmanTreeRelease(HuffmanTree *tree);
int HuffmanTreeConstruct(HuffmanTree *tree, const ArrayList *weights);
int HuffmanTreeCode(const HuffmanTree *tree, HuffmanCode **pcode, unsigned int *size);
#endif
