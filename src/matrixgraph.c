#include "matrixgraph.h"
#include "arraylist.h"
#include "rerror.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void MatrixGraphInit(MatrixGraph *graph, unsigned nvex) {
    graph->arcs = NULL;
    graph->type = 0;
    ArrayListInit(&graph->vexs, nvex);
}

void MatrixGraphRelease(MatrixGraph *graph) {
    free(graph->arcs);
    graph->arcs = NULL;
    ArrayListRelease(&graph->vexs);
}

#define IFOOM(stat) \
    if ((err = stat) < 0) \
        return err;

int MatrixGraphConstruct(MatrixGraph *graph, GraphType type, ArrayList *vexs, unsigned narc, ArcDesc *arcs) {
    int err;
    IFOOM(ArrayListExtend(&graph->vexs, vexs));
    unsigned nvex = ArrayListSize(vexs);
    graph->type = type;
    Arc *arclist;
    if (!(arclist = malloc(sizeof(Arc) * nvex * nvex)))
        return -RERR_OOM;
    if (type & GRAPH)
        memset(arclist, 0, sizeof(Arc) * nvex * nvex);
    else
        memset(arclist, 0xff, sizeof(Arc) * nvex * nvex);

    for (unsigned i = 0; i < narc; i++) {
        arclist[arcs[i].arcfrom * nvex + arcs[i].arcto].weight = (unsigned)arcs[i].weight;
        if (!(type & DIRECTED))
            arclist[arcs[i].arcto * nvex + arcs[i].arcfrom].weight = (unsigned)arcs[i].weight;
    }
    graph->arcs = arclist;
    return -RERR_OK;
}

void MatrixGraphPrint(const MatrixGraph *graph) {
    if (!(graph->type & (GRAPH | NETWORK)))
        return;
    printf("Vexes: ");
    unsigned size = ArrayListSize(&graph->vexs);
    for (char *ptr = (char *)graph->vexs.arr;
        ptr < (char *)(graph->vexs.arr + size); ptr += sizeof(NODE_TYPE))
        printf("%s ", ptr);
    puts("\nArcs:");
    if (graph->type & NETWORK)
        for (unsigned i = 0; i < size; i++) {
            for (unsigned j = 0; j < size; j++)
                if (graph->arcs[i * size + j].weight == INF)
                    printf("INF ");
                else
                    printf("%3d ", graph->arcs[i * size + j].weight);
            putchar('\n');
        }
    else
        for (unsigned i = 0; i < size; i++) {
            for (unsigned j = 0; j < size; j++)
                printf("%d ", graph->arcs[i * size + j].adjacent);
            putchar('\n');
        }
}

int MatrixGraphCalcDegree(const MatrixGraph *graph, ArrayList *degrees) {
    if (!(graph->type & (GRAPH | NETWORK)))
        return -RERR_EMPTY;
    unsigned size = ArrayListSize(&graph->vexs);
    unsigned nul = graph->type & GRAPH ? false : INF;
    for (unsigned from = 0; from < size; from++) {
        unsigned out = 0, in = 0;
        for (unsigned to = 0; to < size; to++) {
            if (graph->arcs[from * size + to].weight != nul)
                out++;
            if (graph->arcs[to * size + from].weight != nul)
                in++;
        }
        int err = ArrayListAppend(degrees, graph->type & DIRECTED ? PACK(in, out) : out);
        if (err)
            return err;
    }
    return -RERR_OK;
}
