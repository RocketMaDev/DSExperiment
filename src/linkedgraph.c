#include "linkedgraph.h"
#include "arraylist.h"
#include "matrixgraph.h"
#include "rerror.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void LinkedGraphInit(LinkedGraph *graph, unsigned narc) {
    graph->linkTable = NULL;
    graph->type = 0;
    ArrayListInit(&graph->vexs, narc);
}

void LinkedGraphRelease(LinkedGraph *graph) {
    if (graph->linkTable) {
        for (unsigned i = 0; i < ArrayListSize(&graph->vexs); i++)
            ArrayListRelease(graph->linkTable + i);
        free(graph->linkTable);
    }
    graph->linkTable = NULL;
    ArrayListClear(&graph->vexs);
}

#define IFOOM(stat) \
    if ((err = stat) < 0) \
        return err;

int LinkedGraphConstruct(LinkedGraph *graph, GraphType type, ArrayList *vexs, unsigned narc, ArcDesc *arcs) {
    int err;
    IFOOM(ArrayListExtend(&graph->vexs, vexs));
    unsigned nvex = ArrayListSize(vexs);
    graph->type = type;
    ArrayList *list;
    if (!(list = malloc(sizeof(ArrayList) * nvex)))
        return -RERR_OOM;
    for (unsigned i = 0; i < nvex; i++)
        if (ArrayListInit(list + i, 0) != -RERR_OK)
            return -RERR_OOM;
    graph->linkTable = list;

    for (unsigned i = 0; i < narc; i++) {
        ArrayListAppend(list + arcs[i].arcfrom, PACK_LINK(arcs[i].weight, arcs[i].arcto));
        if (!(type & DIRECTED))
            ArrayListAppend(list + arcs[i].arcto, PACK_LINK(arcs[i].weight, arcs[i].arcfrom));
    }
    return -RERR_OK;
}

void LinkedGraphPrint(const LinkedGraph *graph) {
    if (!(graph->type & (GRAPH | NETWORK)))
        return;
    unsigned size = ArrayListSize(&graph->vexs);
    long *ptr = graph->vexs.arr;
    for (unsigned i = 0; i < size; i++) {
        printf("%s: [", (char *)(ptr + i));
        unsigned nitem = ArrayListSize(graph->linkTable + i);
        for (unsigned j = 0; j < nitem; j++) {
            long *arr = (graph->linkTable + i)->arr;
            printf("%s", (char *)(ptr + LINK_TO(arr[j])));
            if (graph->type & NETWORK)
                printf("(%d)", (int)LINK_WEIGHT(arr[j]));
            if (j != nitem - 1) {
                putchar(',');
                putchar(' ');
            }
        }
        putchar(']');
        putchar('\n');
    }
}

int LinkedGraphCalcDegree(const LinkedGraph *graph, ArrayList *degrees) {
    if (!(graph->type & (GRAPH | NETWORK)))
        return -RERR_EMPTY;
    unsigned size = ArrayListSize(&graph->vexs);
    for (unsigned from = 0; from < size; from++) {
        register unsigned out = ArrayListSize(graph->linkTable + from);
        int err = ArrayListAppend(degrees, out);
        if (err)
            return err;
    }
    if (!(graph->type & DIRECTED))
        return -RERR_OK;
    register long *arr = degrees->arr;
    for (unsigned from = 0; from < size; from++) {
        register long *links = (graph->linkTable + from)->arr;
        for (unsigned j = 0; j < ArrayListSize(graph->linkTable + from); j++) {
            register long *ptr = arr + LINK_TO(links[j]);
            *ptr = PACK(GET_IN(*ptr) + 1, GET_OUT(*ptr));
        }
    }
    return -RERR_OK;
}
