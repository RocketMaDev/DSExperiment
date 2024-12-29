#include "../src/graphops.h"
#include <stdio.h>

static MatrixGraph x;
static MatrixGraph *graph = &x;
static ArcDesc descs[] = {{0, 1, 10}, {0, 2, 12}, {0, 4, 15},
    {1, 2, 7}, {1, 3, 5}, {1, 5, 6},
    {2, 4, 12}, {2, 5, 8}, {3, 5, 6}, {4, 5, 10}};
static unsigned narc = sizeof(descs) / sizeof(ArcDesc);

static void tst_create_graph(void) {
    puts("=====tst_create_graph=====");
    long buf[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    ArrayList vexs = {buf, 6, 6};
    MatrixGraphConstruct(graph, UNDIRECTED | NETWORK, &vexs, narc, descs);
    MatrixGraphPrint(graph);
    puts("==========================\n");
}

static void tst_mst_prim(void) {
    puts("=====tst_mst_prim=====");
    unsigned initvex = 0;
    PrintMinSpanTree(graph, MST_PRIM, &initvex);
    puts("======================\n");
}

static void tst_mst_kruskal(void) {
    puts("=====tst_mst_kruskal=====");
    DescList desc = {descs, narc};
    PrintMinSpanTree(graph, MST_KRUSKAL, &desc);
    puts("=========================\n");
}

int main(void) {
    tst_create_graph();
    tst_mst_prim();
    tst_mst_kruskal();
    MatrixGraphRelease(graph);
    return 0;
}
