#ifndef __GRAPH_OPS_H__
#define __GRAPH_OPS_H__
#include "matrixgraph.h"
#include "linkedgraph.h"

typedef enum {
    GRAPH_TRAVERSE_DEPTH_FIRST,
    GRAPH_TRAVERSE_BREADTH_FIRST,
} GraphTraverseType;

/**
 * Traverse linked graph with `func`. Uses bitmap to mark visited vertexes, which
 * take `unsigned long long` as the container, so vertexes of graph should be no
 * more than 64. Always choose `vertex[0]` as the first vertex to traverse.
 *
 * @param graph The graph to traverse.
 * @param type  One of enum GraphTraverseType, BFS or DFS.
 * @param buf   An arbitrary pointer which will be `func`'s third arg when called.
 * @param func  The function to traverse. `vex_idx` is the index of vertex traversed
 *              in `graph->vexs`.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_EMPTY            If graph is empty.
 * @exception RERR_NOTIMPLEMENTED   If graph has more than 64 vertexes.
 */
int LinkedGraphTraverse(const LinkedGraph *graph, GraphTraverseType type, void *buf, void (*func)(unsigned vex_idx, void *buf));

typedef enum {
    MST_PRIM,
    MST_KRUSKAL,
} MSTMethod;

typedef struct {
    ArcDesc *descs;
    unsigned size;
} DescList;

/**
 * Print minimum spanning tree of graph.
 * 
 * @param graph     A matrix graph.
 * @param method    One of MSTMethod, prim or kruskal.
 * @param arg       For different method, this arg has different type.
 *                  - MST_PRIM: `const unsigned *`, holding `&initVex` to
 *                    tell which vertex to begin with.
 *                  - MST_KRUSKAL: `const DescList *`, holding a struct to
 *                    tell arc descriptions and the size of `descs`.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_OOM              Insufficient memory when running.
 * @exception RERR_EMPTY            If graph is empty.
 * @exception RERR_NOTIMPLEMENTED   If graph is a network, and graph is directed when
 *                                  running kruskal.
 * @exception RERR_OVERFLOW         If `initVex >= vex.size` when running prim.
 * @exception RERR_CORRUPTED        If graph is not connected when running prim.
 *
 * @note This function print to stdout. After MST_KRUSKAL, `descs` will be sorted
 * by weight, so make sure `descs` is mutable.
 */
int PrintMinSpanTree(const MatrixGraph *graph, MSTMethod method, const void *arg);
#endif
