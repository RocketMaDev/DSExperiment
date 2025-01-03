#include "../src/aoenet.h"
#include <stdio.h>

static LinkedGraph x;
static LinkedGraph *graph = &x;
static ArrayList topo;

static void tst_create_network(void) {
    puts("=====tst_create_network=====");
    long names[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
    ArrayList vexs = {names, sizeof(names) / sizeof(long), sizeof(names) / sizeof(long)};
#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6
#define H 7
#define I 8
#define J 9
#define pack(idx, wei) (((idx) << 8) | (wei))
    ArcDesc descs[] = {{A, B, 8}, {A, C, 6}, {A, E, 7}, {B, D, 3}, {C, D, 10},
        {C, G, 9}, {E, G, 9}, {E, H, 13}, {D, F, 4}, {D, I, 19}, {G, I, 8},
        {G, H, 2}, {H, I, 6}, {F, J, 14}, {I, J, 10}};
    for (unsigned i = 0; i < sizeof(descs) / sizeof(ArcDesc); i++)
        descs[i].weight = pack(i, descs[i].weight);
    LinkedGraphInit(graph, vexs.size);
    LinkedGraphConstruct(graph, DIRECTED | NETWORK, &vexs, sizeof(descs) / sizeof(ArcDesc), descs);
    LinkedGraphPrint(graph);
    puts("============================\n");
}

static void pvex(unsigned idx, long vex, void *buf) {
    (void)buf, (void)idx;
    printf("%s -> ", (const char *)(graph->vexs.arr + vex));
}

static void tst_topological_sort(void) {
    puts("=====tst_topological_sort=====");
    ArrayListInit(&topo, 0);
    TopologicalSort(graph, &topo, NULL);
    printf("topo: [");
    ArrayListTraverse(&topo, NULL, pvex);
    printf("^]\n");
    puts("==============================\n");
}

static void tst_critical_path(void) {
    puts("=====tst_critical_path=====");
    PrintCritialPath(graph, &topo);
    puts("===========================\n");
}

int main(void) {
    tst_create_network();
    tst_topological_sort();
    tst_critical_path();
    ArrayListRelease(&topo);
    LinkedGraphRelease(graph);
    return 0;
}
