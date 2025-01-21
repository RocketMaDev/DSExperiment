/**
 * Chapter 6 - GRAPH
 * Experiment 3.5 - Activity on Edge Network
 */
#ifndef __AOE_NETWORK_H__
#define __AOE_NETWORK_H__
#include "linkedgraph.h"
#include "arraylist.h"
#define LINK_ACT(pack) (((pack) & 0xff00) >> 8)
#define LINK_WEI(pack) ((pack) & 0xff)
#define PACK_ALL(to, act, wei) (((to) << 32) | (act) << 8 | (wei))
/**
 * Sort graph nodes topologically and put them into arraylist.
 *
 * @param graph     A linkedgraph, should be directed.
 * @param topo      An (empty) arraylist to store topological node sequence.
 * @param degrees   Optional calculated degrees of graph. If `NULL`, calc degrees when run.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_EMPTY            If `graph` is empty.
 * @exception RERR_NOTIMPLEMENTED   `graph` is undirected.
 * @exception RERR_OOM              Insufficient memory when running.
 * @exception RERR_CORRUPTED        Ring exists in `graph`, can not perform topological sort.
 */
int TopologicalSort(const LinkedGraph *graph, ArrayList * restrict topo, Nullable const ArrayList * restrict degrees);

/**
 * Print critical path: calculated earliest time & latest time for events,
 * earliest & latest time for activities and linked activities in the last.
 *
 * @param graph A directed network.
 * @param topo  Calculated topological node sequence for `graph`
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_EMPTY            If `graph` is empty or `topo` is empty.
 * @exception RERR_NOTIMPLEMENTED   `graph` is not a directed network.
 * @exception RERR_OOM              Insufficient memory when running.
 *
 * @note This function print to stdout.
 */
int PrintCritialPath(const LinkedGraph *graph, const ArrayList *topo);
#endif
