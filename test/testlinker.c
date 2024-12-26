#include "../src/linkedgraph.h"
#include <stdio.h>

static LinkedGraph graphs[4];
static LinkedGraph *ug = graphs + 0; // undirected graph
static LinkedGraph *dg = graphs + 1; // directed graph
static LinkedGraph *un = graphs + 2; // undirected network
static LinkedGraph *dn = graphs + 3; // directed network

static int mkug(void) {
    ArcDesc desc[] = {{0, 1, 1}, {0, 2, 1}, {0, 3, 1}, {2, 1, 1}, {2, 3, 1}};
    long arr[] = {0x3056, 0x3156, 0x3256, 0x3356};
    ArrayList vexs = {arr, 4, 4};
    return LinkedGraphConstruct(ug, UNDIRECTED | GRAPH, &vexs, 5, desc);
}

static int mkdg(void) {
    ArcDesc desc[] = {{0, 1, 1}, {1, 2, 1}, {1, 0, 1}, {2, 0, 1}, {2, 1, 1}, {3, 1, 1}};
    long arr[] = {0x3056, 0x3156, 0x3256, 0x3356};
    ArrayList vexs = {arr, 4, 4};
    return LinkedGraphConstruct(dg, DIRECTED | GRAPH, &vexs, 6, desc);
}

static int mkun(void) {
    ArcDesc desc[] = {{0, 1, 56}, {0, 2, 34}, {0, 3, 78}, {2, 3, 25}};
    long arr[] = {0x3056, 0x3156, 0x3256, 0x3356};
    ArrayList vexs = {arr, 4, 4};
    return LinkedGraphConstruct(un, UNDIRECTED | NETWORK, &vexs, 4, desc);
}

static int mkdn(void) {
    ArcDesc desc[] = {{0, 2, 50}, {1, 2, 45}, {2, 0, 64}};
    long arr[] = {0x3056, 0x3156, 0x3256};
    ArrayList vexs = {arr, 3, 3};
    return LinkedGraphConstruct(dn, DIRECTED | NETWORK, &vexs, 3, desc);
}

static void tst_linked_graph_construct(void) {
    puts("=====tst_linked_graph_construct=====");
    LinkedGraphInit(ug, 5);
    LinkedGraphInit(dg, 6);
    LinkedGraphInit(un, 4);
    LinkedGraphInit(dn, 3);
    
    int err = 0;
    err += mkug();
    err += mkdg();
    err += mkun();
    err += mkdn();
    printf("Constructed 4 graphs with status %d.\n", err);
    puts("====================================\n");
}

static void tst_linked_graph_print(void) {
    puts("=====tst_linked_graph_print=====");
    puts("ug:");
    LinkedGraphPrint(ug);
    puts("dg:");
    LinkedGraphPrint(dg);
    puts("un:");
    LinkedGraphPrint(un);
    puts("dn:");
    LinkedGraphPrint(dn);
    puts("================================\n");
}

static void pdirected(unsigned i, long v, void *buf) {
    printf("Node(\"%s\", in = %ld, out = %ld)\n", (char *)((long *)buf + i), GET_IN(v), GET_OUT(v));
}

static void pundirected(unsigned i, long v, void *buf) {
    printf("Node(\"%s\", deg = %ld)\n", (char *)((long *)buf + i), v);
}

static void degree_print(const LinkedGraph *graph, ArrayList *degrees) {
    register long *chrs = graph->vexs.arr;
    if (graph->type & DIRECTED)
        ArrayListTraverse(degrees, chrs, pdirected);
    else
        ArrayListTraverse(degrees, chrs, pundirected);
}

static void tst_linked_graph_degree(void) {
    puts("=====tst_linked_graph_degree=====");
    ArrayList ugDegree, dgDegree, unDegree, dnDegree;
    ArrayListInit(&ugDegree, 0);
    ArrayListInit(&dgDegree, 0);
    ArrayListInit(&unDegree, 0);
    ArrayListInit(&dnDegree, 0);
    LinkedGraphCalcDegree(ug, &ugDegree);
    LinkedGraphCalcDegree(dg, &dgDegree);
    LinkedGraphCalcDegree(un, &unDegree);
    LinkedGraphCalcDegree(dn, &dnDegree);

#define degree_print(g) \
    puts("Degrees of " #g ":"); \
    degree_print(g, &g##Degree);
    degree_print(ug);
    degree_print(dg);
    degree_print(un);
    degree_print(dn);

    ArrayListRelease(&ugDegree);
    ArrayListRelease(&dgDegree);
    ArrayListRelease(&unDegree);
    ArrayListRelease(&dnDegree);
    puts("=================================\n");
}

int main(void) {
    tst_linked_graph_construct();
    tst_linked_graph_print();
    tst_linked_graph_degree();
    LinkedGraphRelease(ug);
    LinkedGraphRelease(dg);
    LinkedGraphRelease(un);
    LinkedGraphRelease(dn);
    return 0;
}
