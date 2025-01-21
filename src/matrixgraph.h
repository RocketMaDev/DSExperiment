#ifndef __MATRIX_GRAPH_H__
#define __MATRIX_GRAPH_H__
#include <stdbool.h>
#define NODE_TYPE long
#include "arraylist.h"

#define INF (unsigned)-1
typedef enum {
    UNDIRECTED = 0,
    DIRECTED = 1,
    GRAPH = 2,
    NETWORK = 4,
} GraphType;

typedef union {
    bool adjacent;
    unsigned weight;
} Arc;

typedef struct {
    unsigned char arcfrom;
    unsigned char arcto;
    unsigned short weight;
} ArcDesc;

typedef struct {
    ArrayList vexs;
    Arc *arcs;
    GraphType type;
} MatrixGraph;

/**
 * Initialize a matrix graph with the amount of vertexes.
 *
 * @param graph An uninitialized graph.
 * @param nvex  The amount of vertexes.
 * @return A `-status` code where `status` is one of member in `RERROR`. `0` if no error.
 *
 * @exception RERR_OOM Insufficient memory when running.
 */
int MatrixGraphInit(MatrixGraph *graph, unsigned nvex);
/**
 * Releases the graph. Destructor.
 *
 * @param graph The graph to release.
 */
void MatrixGraphRelease(MatrixGraph *graph);

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
int MatrixGraphConstruct(MatrixGraph *graph, GraphType type, ArrayList *vexs, unsigned narc, ArcDesc *arcs);
/**
 * Print the graph.
 *
 * @param graph The graph to print.
 *
 * @note This function print to stdout.
 */
void MatrixGraphPrint(const MatrixGraph *graph);

#define PACK(in, out) ((in) << 16 | (out))
#define GET_IN(val) ((val) >> 16)
#define GET_OUT(val) ((val) & 0xffff)

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
int MatrixGraphCalcDegree(const MatrixGraph *graph, ArrayList *degrees);
#endif
