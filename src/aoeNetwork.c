#include "aoeNetwork.h"
#include "linkedgraph.h"
#include "arraylist.h"
#include "matrixgraph.h"
#include "stack.h"
#include "rerror.h"
#include <stdlib.h>
#include <stdio.h>

#define IFERR(stat) \
    do { \
        err = stat; \
        if (err) \
            goto cleanup; \
    } while (0)

struct degstack {
    long *degrees;
    ArrayList *stack;
};

static void itrop(unsigned idx, long v, void *buf) {
    // v is linked vex
    (void)idx;
    register struct degstack *ds = buf;
    ds->degrees[LINK_TO(v)]--;
    if (!ds->degrees[LINK_TO(v)] && StackPush(ds->stack, LINK_TO(v)))
        OUT_OF_ENUM("Unrecoverable OOM happens when performing TopologicalSort");
}

int TopologicalSort(const LinkedGraph *graph, ArrayList * restrict topo, Nullable const ArrayList * restrict degrees) {
    if (graph->linkTable == NULL)
        return -RERR_EMPTY;
    if (!(graph->type & DIRECTED))
        return -RERR_NOTIMPLEMENTED;

    int err;
    ArrayList deg;
    ArrayListInit(&deg, 0);
    ArrayList stack;
    ArrayListInit(&stack, 0);
    if (degrees)
        IFERR(ArrayListExtend(&deg, degrees));
    else
        IFERR(LinkedGraphCalcDegree(graph, &deg));

    for (unsigned i = 0; i < deg.size; i++) {
        deg.arr[i] = GET_IN(deg.arr[i]); // discard out degree to speed up
        if (!deg.arr[i])
            IFERR(StackPush(&stack, i));
    }
    struct degstack ds = {deg.arr, &stack};
    while (stack.size) { // not empty
        long x;
        StackPop(&stack, &x);
        unsigned vex = x;
        IFERR(ArrayListAppend(topo, vex));
        ArrayListTraverse(graph->linkTable + vex, &ds, itrop);
    }
    if (topo->size < graph->vexs.size)
        err = -RERR_CORRUPPTED;
    else
        err = -RERR_OK;
cleanup:
    ArrayListRelease(&deg);
    ArrayListRelease(&stack);
    return err;
}

int PrintCritialPath(const LinkedGraph *graph, const ArrayList *topo) {
    if (graph->linkTable == NULL || topo->size == 0)
        return -RERR_EMPTY;
    if (!(graph->type & (NETWORK | DIRECTED)))
        return -RERR_NOTIMPLEMENTED;
    register unsigned nvex = graph->vexs.size;
    register long *topovex = topo->arr;
    int err;
    unsigned *earliest = malloc(sizeof(unsigned) * nvex);
    unsigned *latest = malloc(sizeof(unsigned) * nvex);
    if (earliest == NULL || latest == NULL) {
        err = -RERR_OOM;
        goto cleanup;
    }

    for (unsigned i = 0; i < nvex; i++) {
        unsigned vex = topovex[i];
        register ArrayList *links = graph->linkTable + vex;
        for (unsigned j = 0; j < links->size; j++)
            if (earliest[LINK_TO(links->arr[j])] < earliest[vex] + LINK_WEIGHT(links->arr[j]))
                earliest[LINK_TO(links->arr[j])] = earliest[vex] + LINK_WEIGHT(links->arr[j]);
    }
#ifdef DEBUG
    printf("Calculated earliest: [");
    for (unsigned i = 0; i < nvex - 1; i++)
        printf("%d, ", earliest[i]);
    printf("%d]\n", earliest[nvex - 1]);
#endif
    for (unsigned i = 0; i < nvex; i++)
        latest[i] = earliest[nvex - 1];

    for (int i = (int)nvex - 1; i >= 0; i--) {
        unsigned vex = topovex[i];
        register ArrayList *links = graph->linkTable + vex;
        for (unsigned j = 0; j < links->size; j++)
            if (latest[vex] > latest[LINK_TO(links->arr[j])] - LINK_WEIGHT(links->arr[j]))
                latest[vex] = latest[LINK_TO(links->arr[j])] - LINK_WEIGHT(links->arr[j]);
    }
#ifdef DEBUG
    printf("Calculated latest: [");
    for (unsigned i = 0; i < nvex - 1; i++)
        printf("%d, ", latest[i]);
    printf("%d]\n", latest[nvex - 1]);
#endif

    for (unsigned i = 0; i < nvex; i++) {
        register ArrayList *links = graph->linkTable + i;
        for (unsigned ji = 0; ji < links->size; ji++)
            if (earliest[i] == latest[LINK_TO(links->arr[ji])] - LINK_WEIGHT(links->arr[ji])) {
                unsigned j = LINK_TO(links->arr[ji]);
                unsigned v1idx = ArrayListFind(topo, i);
                unsigned v2idx = ArrayListFind(topo, j);
                if (v1idx > v2idx) { // j is prior to i, reverse them
                    unsigned t = i;
                    i = j;
                    j = t;
                }
                printf("%s -> %s\n", (char *)(graph->vexs.arr + i), (char *)(graph->vexs.arr + j));
            }
    }
cleanup:
    free(earliest);
    free(latest);
    return err;
}
