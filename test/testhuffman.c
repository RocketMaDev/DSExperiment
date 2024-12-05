#include "../src/huffman.h"
#include <stdlib.h>
#include <stdio.h>

static HuffmanTree *huffman;
static unsigned size;

int arr[] = {5, 29, 7, 8, 14, 23, 3, 11};
ArrayList weights = {arr, sizeof(arr) / sizeof(int), sizeof(arr) / sizeof(int)};

static void tst_huffman_construct(void) {
    puts("=====tst_huffman_construct=====");
    size = weights.size;
    int err = HuffmanTreeConstruct(huffman, &weights);
    printf("Construct huffman tree with status %d\n", err);
    puts("===============================\n");
}

static void pnode(const TreeNode *node, void *buf) {
    (void)buf;
    putchar(INDEX(node->data) < size ? '[' : ' ');
    printf("%2d|%3d|%2d", node->lnode ? WEIGHT(node->lnode->data) : (unsigned)-1,
                          WEIGHT(node->data),
                          node->rnode ? WEIGHT(node->rnode->data) : (unsigned)-1);
    putchar(INDEX(node->data) < size ? ']' : ' ');
    putchar('\n');
}

static void tst_huffman_print(void) {
    puts("=====tst_huffman_print=====");
    puts("Constructed huffman tree looks like:");
    TreeTraverse(&huffman->tree, PRE_ORDER, NULL, pnode);
    puts("===========================\n");
}

static void tst_huffman_code(void) {
    puts("=====tst_huffman_code=====");
    puts("node weight: node encoding");
    for (unsigned i = 0; i < size; i++) {
        printf("%2d: ", huffman->weights->arr[i]);
        HuffmanCode code = huffman->codes[i];
        for (unsigned j = 0; j < code.nbit; j++) {
            putchar('0' + (code.bits & 1));
            code.bits >>= 1;
        }
        putchar('\n');
    }
    puts("==========================\n");
}

int main(void) {
    huffman = malloc(sizeof(HuffmanTree));
    HuffmanTreeInit(huffman);
    tst_huffman_construct();
    tst_huffman_print();
    tst_huffman_code();
    HuffmanTreeRelease(huffman);
    free(huffman);
    return 0;
}
