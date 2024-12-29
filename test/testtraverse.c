#include "../src/graphops.h"
#include <stdio.h>
static LinkedGraph x;
static LinkedGraph *graph = &x;

static void tst_create_graph(void) {
    puts("=====tst_create_graph=====");
    LinkedGraphInit(graph, 6);
    ArcDesc descs[] = {{0, 1, 1}, {0, 2, 1}, {1, 3, 1}, {1, 4, 1}, {2, 5, 1}, {4, 3, 1}, {4, 5, 1}};
    long buf[] = {0x3043, 0x3143, 0x3243, 0x3343, 0x3443, 0x3543};
    ArrayList vexs = {buf, 6, 6};
    LinkedGraphConstruct(graph, UNDIRECTED | GRAPH, &vexs, 7, descs);
    LinkedGraphPrint(graph);
    puts("==========================\n");
}

static void pvex(unsigned vex, void *buf) {
    (void)buf;
    printf("%s -> ", (const char *)(graph->vexs.arr + vex));
}

static void tst_bfs_traverse(void) {
    puts("=====tst_bfs_traverse=====");
    LinkedGraphTraverse(graph, GRAPH_TRAVERSE_BREADTH_FIRST, NULL, pvex);
    putchar('^');
    putchar('\n');
    puts("==========================\n");
}

static void tst_dfs_traverse(void) {
    puts("=====tst_dfs_traverse=====");
    LinkedGraphTraverse(graph, GRAPH_TRAVERSE_DEPTH_FIRST, NULL, pvex);
    putchar('^');
    putchar('\n');
    puts("==========================\n");
}

int main(void) {
    tst_create_graph();
    tst_dfs_traverse();
    tst_bfs_traverse();
    LinkedGraphRelease(graph);
    return 0;
}
