#include "../src/huffman.h"
#include <stdlib.h>
#include <stdio.h>

static HuffmanTree *huffman;
static unsigned size;

int arr[] = {5, 29, 7, 8, 14, 23, 3, 11};
ArrayList weights = {arr, sizeof(arr) / sizeof(int), sizeof(arr) / sizeof(int)};

static unsigned searchParent(const TreeNode **nodes, unsigned start, unsigned size, const TreeNode *target) {
    for (unsigned i = start; i < size; i++)
        if (nodes[i]->lnode == target || nodes[i]->rnode == target)
            return i + 1;
    return 0;
}

static unsigned searchIndex(const TreeNode **nodes, unsigned size, const TreeNode *target) {
    if (!target)
        return 0;
    for (unsigned i = 0; i < size; i++)
        if (nodes[i] == target)
            return i + 1;
    return 0;
}

static void tst_huffman_construct(void) {
    puts("=====tst_huffman_construct=====");
    size = weights.size;
    int *arr = weights.arr;
    int err = HuffmanTreeConstruct(huffman, &weights);
    printf("Construct huffman tree with status %d\n", err);
    puts("Inited huffman tree looks like:");
    for (unsigned i = 0; i < size; i++)
        printf("%2d|%3d| 0| 0| 0\n", i + 1, arr[i]);
    for (unsigned i = size; i < size * 2 - 1; i++)
        printf("%2d| - | 0| 0| 0\n", i + 1);
    puts("===============================\n");
}

static void tst_huffman_print(void) {
    puts("=====tst_huffman_print=====");
    puts("Constructed huffman tree looks like:");
    const TreeNode **nodes = (const TreeNode **)huffman->nodes;
    const unsigned base = ArrayListSize(huffman->weights), size = 2 * base - 1;
    for (unsigned i = 0; i < size; i++)
        printf("%2d|%3d|%2d|%2d|%2d\n", i + 1, WEIGHT(nodes[i]->data),
                searchParent(nodes, base, size, nodes[i]),
                searchIndex(nodes, size, nodes[i]->lnode),
                searchIndex(nodes, size, nodes[i]->rnode));
    /* TreeTraverse(&huffman->tree, PRE_ORDER, NULL, pnode); */
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
