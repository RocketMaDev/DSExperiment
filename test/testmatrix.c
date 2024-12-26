#include "../src/matrixgraph.h"
#include <stdio.h>

static MatrixGraph graphs[4];
static MatrixGraph *ug = graphs + 0; // undirected graph
static MatrixGraph *dg = graphs + 1; // directed graph
static MatrixGraph *un = graphs + 2; // undirected network
static MatrixGraph *dn = graphs + 3; // directed network

static int mkug(void) {
    ArcDesc desc[] = {{0, 1, 1}, {0, 2, 1}, {0, 3, 1}, {2, 1, 1}, {2, 3, 1}};
    long arr[] = {0x3056, 0x3156, 0x3256, 0x3356};
    ArrayList vexs = {arr, 4, 4};
    return MatrixGraphConstruct(ug, UNDIRECTED | GRAPH, &vexs, 5, desc);
}

static int mkdg(void) {
    ArcDesc desc[] = {{0, 1, 1}, {1, 2, 1}, {1, 0, 1}, {2, 0, 1}, {2, 1, 1}, {3, 1, 1}};
    long arr[] = {0x3056, 0x3156, 0x3256, 0x3356};
    ArrayList vexs = {arr, 4, 4};
    return MatrixGraphConstruct(dg, DIRECTED | GRAPH, &vexs, 6, desc);
}

static int mkun(void) {
    ArcDesc desc[] = {{0, 1, 56}, {0, 2, 34}, {0, 3, 78}, {2, 3, 25}};
    long arr[] = {0x3056, 0x3156, 0x3256, 0x3356};
    ArrayList vexs = {arr, 4, 4};
    return MatrixGraphConstruct(un, UNDIRECTED | NETWORK, &vexs, 4, desc);
}

static int mkdn(void) {
    ArcDesc desc[] = {{0, 2, 50}, {1, 2, 45}, {2, 0, 64}};
    long arr[] = {0x3056, 0x3156, 0x3256};
    ArrayList vexs = {arr, 3, 3};
    return MatrixGraphConstruct(dn, DIRECTED | NETWORK, &vexs, 3, desc);
}

static void tst_matrix_graph_construct(void) {
    puts("=====tst_matrix_graph_construct=====");
    MatrixGraphInit(ug, 5);
    MatrixGraphInit(dg, 6);
    MatrixGraphInit(un, 4);
    MatrixGraphInit(dn, 3);
    
    int err = 0;
    err += mkug();
    err += mkdg();
    err += mkun();
    err += mkdn();
    printf("Constructed 4 graphs with status %d.\n", err);
    puts("====================================\n");
}

static void tst_matrix_graph_print(void) {
    puts("=====tst_matrix_graph_print=====");
    puts("ug:");
    MatrixGraphPrint(ug);
    puts("dg:");
    MatrixGraphPrint(dg);
    puts("un:");
    MatrixGraphPrint(un);
    puts("dn:");
    MatrixGraphPrint(dn);
    puts("================================\n");
}

static void pdirected(unsigned i, long v, void *buf) {
    printf("Node(\"%s\", in = %ld, out = %ld)\n", (char *)((long *)buf + i), GET_IN(v), GET_OUT(v));
}

static void pundirected(unsigned i, long v, void *buf) {
    printf("Node(\"%s\", deg = %ld)\n", (char *)((long *)buf + i), v);
}

static void degree_print(const MatrixGraph *graph, ArrayList *degrees) {
    register long *chrs = graph->vexs.arr;
    if (graph->type & DIRECTED)
        ArrayListTraverse(degrees, chrs, pdirected);
    else
        ArrayListTraverse(degrees, chrs, pundirected);
}

static void tst_matrix_graph_degree(void) {
    puts("=====tst_matrix_graph_degree=====");
    ArrayList ugDegree, dgDegree, unDegree, dnDegree;
    ArrayListInit(&ugDegree, 0);
    ArrayListInit(&dgDegree, 0);
    ArrayListInit(&unDegree, 0);
    ArrayListInit(&dnDegree, 0);
    MatrixGraphCalcDegree(ug, &ugDegree);
    MatrixGraphCalcDegree(dg, &dgDegree);
    MatrixGraphCalcDegree(un, &unDegree);
    MatrixGraphCalcDegree(dn, &dnDegree);

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
    tst_matrix_graph_construct();
    tst_matrix_graph_print();
    tst_matrix_graph_degree();
    MatrixGraphRelease(ug);
    MatrixGraphRelease(dg);
    MatrixGraphRelease(un);
    MatrixGraphRelease(dn);
    return 0;
}
