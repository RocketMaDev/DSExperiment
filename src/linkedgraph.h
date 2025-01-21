#ifndef __LINKED_GRAPH_H__
#define __LINKED_GRAPH_H__
#include <stdbool.h>
#include "matrixgraph.h"
#include "arraylist.h"

#define INF (unsigned)-1
typedef struct {
    ArrayList vexs;
    ArrayList *linkTable;
    GraphType type;
} LinkedGraph;

#define PACK_LINK(weight, to) (((long)to) << 32 | weight)
#define LINK_WEIGHT(pack) ((pack) & 0xffffffff)
#define LINK_TO(pack) ((pack) >> 32)

/**
 * Initialize a linked graph with the amount of vertexes.
 *
 * @param graph An uninitialized graph.
 * @param nvex  The amount of vertexes.
 * @return A `-status` code where `status` is one of member in `RERROR`. `0` if no error.
 *
 * @exception RERR_OOM Insufficient memory when running.
 */
int LinkedGraphInit(LinkedGraph *graph, unsigned nvex);
/**
 * Releases the graph. Destructor.
 *
 * @param graph The graph to release.
 */
void LinkedGraphRelease(LinkedGraph *graph);

/**
 * Construct the graph with given arcs. And names of vertexes is stored
 * in `long`s, as `long` holds 8 bytes on 64bit machines. For example,
 * `"v0"` is converted into 0x3076.
 *
 * @param graph An empty, initialized graph.
 * @param type  One of graph types.
 * @param vexs  An array list to store vertex names.
 * @param narc  Length of `arcs`.
 * @param arcs  Describe arcs in the graph.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_OOM  Insufficient memory when running.
 */
int LinkedGraphConstruct(LinkedGraph *graph, GraphType type, ArrayList *vexs, unsigned narc, ArcDesc *arcs);
/**
 * Print the graph.
 *
 * @param graph The graph to print.
 *
 * @note This function print to stdout.
 */
void LinkedGraphPrint(const LinkedGraph *graph);

/**
 * Calculate the degree of graph and store results in `degrees`. For
 * directed graph/network, degrees will be put in 2 parts: low 16bits:
 * out degree, high 16bits: in degree.
 *
 * @param graph     A graph.
 * @param degrees   An empty, initialized array list.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_EMPTY The graph is uninitialized.
 * @exception RERR_OOM   Insufficient memory when running.
 */
int LinkedGraphCalcDegree(const LinkedGraph *graph, ArrayList *degrees);
#endif

