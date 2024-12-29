#include "graphops.h"
#include "arraylist.h"
#include "linkedgraph.h"
#include "matrixgraph.h"
#include "rerror.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#define QUEUE_SIZE 64
#include "queue.h"

#define visit(var, idx) (var) |= 1 << (idx)
#define isvisited(var, idx) ((var) & (1 << (idx)))

int bfs_traverse(const LinkedGraph *graph, void *buf, void (*func)(unsigned vex_idx, void *buf)) {
    if (graph->vexs.size == 0)
        return -RERR_EMPTY;
    if (graph->vexs.size > 64)
        return -RERR_NOTIMPLEMENTED;
    unsigned long long bits = 0; // max 64 items
    Queue tovisit;
    QueueInit(&tovisit);
    Enqueue(&tovisit, 0); // select 0 as the first elem to visit
    visit(bits, 0);
    while (QueueLength(&tovisit)) {
        long visitor;
        Dequeue(&tovisit, &visitor);
        func(visitor, buf);
        ArrayList *links = graph->linkTable + visitor;
        for (long *arc = links->arr; arc < links->arr + links->size; arc++)
            if (!isvisited(bits, LINK_TO(*arc))) {
                Enqueue(&tovisit, LINK_TO(*arc));
                visit(bits, LINK_TO(*arc));
            }
    }
    return -RERR_OK;
}

static void traverse(const LinkedGraph *graph, void *buf, void (*func)(unsigned ver_idx, void *buf),
        unsigned long long *bits, unsigned idx) {
    visit(*bits, idx);
    func(idx, buf);
    ArrayList *links = graph->linkTable + idx;
    for (long *arc = links->arr; arc < links->arr + links->size; arc++)
        if (!isvisited(*bits, LINK_TO(*arc)))
            traverse(graph, buf, func, bits, LINK_TO(*arc));
}

int dfs_traverse(const LinkedGraph *graph, void *buf, void (*func)(unsigned vex_idx, void *buf)) {
    if (graph->vexs.size == 0)
        return -RERR_EMPTY;
    if (graph->vexs.size > 64)
        return -RERR_NOTIMPLEMENTED;
    unsigned long long bits = 0; // max 64 items
    traverse(graph, buf, func, &bits, 0); // start from 0
    return -RERR_OK;
}

int LinkedGraphTraverse(const LinkedGraph *graph, GraphTraverseType type, void *buf, void (*func)(unsigned vex_idx, void *buf)) {
    if (type == GRAPH_TRAVERSE_DEPTH_FIRST)
        return dfs_traverse(graph, buf, func);
    else if (type == GRAPH_TRAVERSE_BREADTH_FIRST)
        return bfs_traverse(graph, buf, func);
    else
        OUT_OF_ENUM("Unknown GraphTraverseType");
}

struct closedge {
    unsigned adjvex;
    unsigned lowcost;
};

#define VEX_NAME(idx) (const char *)(vexnames + (idx))

__always_inline
static void closedge_print(const long *vexnames, const struct closedge *closedge, unsigned size) {
    printf("|adj vex|");
    for (unsigned i = 0; i < size; i++)
        if (!closedge[i].lowcost)
            printf("  |");
        else
            printf("%2s|", VEX_NAME(closedge[i].adjvex));
    putchar('\n');
    printf("|lowcost|");
    for (unsigned i = 0; i < size; i++)
        if (closedge[i].lowcost == INF)
            printf(" x|");
        else
            printf("%2d|", closedge[i].lowcost);
    putchar('\n');
}

static int mst_prim(const MatrixGraph *graph, unsigned initVex) {
    register unsigned size = graph->vexs.size;
    register Arc *map = graph->arcs;
    register const long *vexnames = graph->vexs.arr;
    if (initVex >= size)
        return -RERR_OVERFLOW;

    // init stage
    struct closedge *closedge = malloc(sizeof(struct closedge) * size);
    closedge[initVex].lowcost = 0;
    for (unsigned i = 0; i < size; i++)
        if (i != initVex)
            closedge[i] = (struct closedge){initVex, map[initVex * size + i].weight};
    closedge_print(vexnames, closedge, size);

    // build mst loop
    for (unsigned i = 1; i < size; i++) {
        // min
        unsigned minWeight = INF, minVex = INF, adjVex;
        for (unsigned j = 0; j < size; j++)
            if (closedge[j].lowcost != 0 && closedge[j].lowcost < minWeight) {
                minVex = j;
                minWeight = closedge[j].lowcost;
            }
        if (minVex == INF) { // no vex choosed, mst can not be built
            free(closedge);
            return -RERR_CORRUPPTED;
        }

        adjVex = closedge[minVex].adjvex; // minVex is not connected, going to connect to adjVex
        printf("#%d: <%s -> %s> (%d)\n", i, VEX_NAME(adjVex), VEX_NAME(minVex),
                map[adjVex * size + minVex].weight);
        closedge[minVex].lowcost = 0; // minVex is now connected

        // update shortest arcs
        for (unsigned j = 0; j < size; j++)
            if (map[minVex * size + j].weight < closedge[j].lowcost)
                closedge[j] = (struct closedge){minVex, map[minVex * size + j].weight};
        closedge_print(vexnames, closedge, size);
    }
    free(closedge);
    return -RERR_OK;
}

static int desccmp(const void *a, const void *b) {
    return ((const ArcDesc *)a)->weight - ((const ArcDesc *)b)->weight;
}

static int mst_kruskal(const MatrixGraph *graph, ArcDesc *descs, unsigned descSize) {
    if (graph->type & DIRECTED)
        return -RERR_NOTIMPLEMENTED;
    register const long *vexnames = graph->vexs.arr;
    register unsigned size = graph->vexs.size;
    qsort(descs, descSize, sizeof(ArcDesc), desccmp);
    // print sorted descs
    puts("Sorted descs: [");
    for (unsigned i = 0; i < descSize; i++)
        printf("    <%s <-> %s> (%d)\n",
            VEX_NAME(descs[i].arcfrom), VEX_NAME(descs[i].arcto), descs[i].weight);
    puts("]");
    unsigned *connvex = malloc(sizeof(unsigned) * size);
    for (unsigned i = 0; i < size; i++)
        connvex[i] = i;

    for (unsigned i = 0; i < descSize; i++) {
        unsigned from = descs[i].arcfrom;
        unsigned to = descs[i].arcto;
        register unsigned conn = connvex[to]; // cache here in case connvex[to] is overwritten later
        if (connvex[from] != connvex[to]) {
            for (unsigned j = 0; j < size; j++)
                if (connvex[j] == conn)
                    connvex[j] = connvex[from];
            printf("Connecting %s & %s, groups: [", VEX_NAME(from), VEX_NAME(to));
            for (unsigned j = 0; j < size - 1; j++)
                printf("%d, ", connvex[j]);
            printf("%d]\n", connvex[size - 1]);
        }
    }
    free(connvex);
    return -RERR_OK;
}

int PrintMinSpanTree(const MatrixGraph *graph, MSTMethod method, const void *arg) {
    if (graph->arcs == NULL)
        return -RERR_EMPTY;
    if (!(graph->type & NETWORK))
        return -RERR_NOTIMPLEMENTED;

    if (method == MST_PRIM)
        return mst_prim(graph, *(unsigned *)arg);
    else if (method == MST_KRUSKAL)
        return mst_kruskal(graph, ((DescList *)arg)->descs, ((DescList *)arg)->size);
    else
        OUT_OF_ENUM("Unknown method");
}
