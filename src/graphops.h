#ifndef __GRAPH_OPS_H__
#define __GRAPH_OPS_H__
#include "matrixgraph.h"
#include "linkedgraph.h"

typedef enum {
    GRAPH_TRAVERSE_DEPTH_FIRST,
    GRAPH_TRAVERSE_BREADTH_FIRST,
} GraphTraverseType;

/* max vexs number: 64 */
int LinkedGraphTraverse(const LinkedGraph *graph, GraphTraverseType type, void *buf, void (*func)(unsigned vex_idx, void *buf));

typedef enum {
    MST_PRIM,
    MST_KRUSKAL,
} MSTMethod;

typedef struct {
    ArcDesc *descs;
    unsigned size;
} DescList;

/* for MST_PRIM, arg is (const unsigned *), holding &initVex;
 * for MST_KRUSKAL, arg is (const DescList *), holding such a struct
 * Note that after MST_KRUSKAL, descs will be sorted by weight */
int PrintMinSpanTree(const MatrixGraph *graph, MSTMethod method, const void *arg);
#endif
