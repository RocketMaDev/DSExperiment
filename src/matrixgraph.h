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

void MatrixGraphInit(MatrixGraph *graph, unsigned nvex);
void MatrixGraphRelease(MatrixGraph *graph);

int MatrixGraphConstruct(MatrixGraph *graph, GraphType type, ArrayList *vexs, unsigned narc, ArcDesc *arcs);
void MatrixGraphPrint(const MatrixGraph *graph);

#define PACK(in, out) ((in) << 16 | (out))
#define GET_IN(val) ((val) >> 16)
#define GET_OUT(val) ((val) & 0xffff)

int MatrixGraphCalcDegree(const MatrixGraph *graph, ArrayList *degrees);
#endif
