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

void LinkedGraphInit(LinkedGraph *graph, unsigned narc);
void LinkedGraphRelease(LinkedGraph *graph);

int LinkedGraphConstruct(LinkedGraph *graph, GraphType type, ArrayList *vexs, unsigned narc, ArcDesc *arcs);
void LinkedGraphPrint(const LinkedGraph *graph);

int LinkedGraphCalcDegree(const LinkedGraph *graph, ArrayList *degrees);
#endif

