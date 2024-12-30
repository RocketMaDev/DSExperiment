#ifndef __AOE_NETWORK_H__
#define __AOE_NETWORK_H__
#include "linkedgraph.h"
#include "arraylist.h"
/* if degrees is NULL, calculate degrees */
int TopologicalSort(const LinkedGraph *graph, ArrayList * restrict topo, Nullable const ArrayList * restrict degrees);

/* if path is NULL, print out the path, or else add it to path. */
int PrintCritialPath(const LinkedGraph *graph, const ArrayList *topo);
#endif
