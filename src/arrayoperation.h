#ifndef __ARRAY_OPERATION_H__
#define __ARRAY_OPERATION_H__

#include "rerror.h"
#include "arraylist.h"
//////////////////////
//// EXPERIMENT 1 ////
//////////////////////
/**
 * Treat `a` and `b` as sets and perform a union operation on `a` and `b`.
 *
 * @param a     A list.
 * @param b     Another list.
 * @param ans   The list to store result. If `NULL`, store result in `a`.
 *
 * @exception RERR_OOM Insufficient memory when running.
 */
int Union(ArrayList *a, const ArrayList *b, Nullable ArrayList *ans);
/**
 * Treat `a` and `b` as sets and perform a intersection operation on `a` and `b`.
 *
 * @param a     A list.
 * @param b     Another list.
 * @param ans   The list to store result. If `NULL`, store result in `a`.
 *
 * @exception RERR_OOM Insufficient memory when running.
 */
int Intersection(ArrayList *a, const ArrayList *b, Nullable ArrayList *ans);
/**
 * Treat `a` and `b` as sets and perform a difference operation on `a` and `b`.
 *
 * @param a     A list.
 * @param b     Another list.
 * @param ans   The list to store result. If `NULL`, store result in `a`.
 *
 * @exception RERR_OOM Insufficient memory when running.
 */
int Difference(ArrayList *a, const ArrayList *b, Nullable ArrayList *ans);

/**
 * Assume elements in `a` and `b` are in non-decreasing order, join them into
 * one list.
 *
 * @param a     A list.
 * @param b     Another list.
 * @param ans   The list to store result. If `NULL`, store result in `a`.
 *
 * @exception RERR_OOM Insufficient memory when running.
 */
int MergeList(ArrayList *a, const ArrayList *b, Nullable ArrayList *res);

/**
 * Remove duplicate elements in `list`.
 *
 * @param list The list to purge. Note that `arr` in list will vary.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_OOM Insufficient memory when running.
 */
int Purge(ArrayList *list);

//////////////////////
//// EXPERIMENT 4 ////
//////////////////////
/**
 * Find element in list linearly. This function is basically wrapper of `ArrayListFind`.
 * To meet the requirements on textbook, that is, a guard is placed at list[0] and
 * then perform a reversal search to optimize searching speed (no need to compare with size),
 * `cmpTimes` is adjusted.
 *
 * @param list      A list to search from.
 * @param target    The element to search.
 * @param cmpTimes  If not `NULL`, store the comparison times.
 * @return The index of left most found element, or `-RERR_NOTFOUND`.
 *
 * @exception RERR_NOTFOUND `target` not found in list.
 */
int ArrayListLinearFind(const ArrayList *list, NODE_TYPE target, Nullable unsigned *cmpTimes);
/**
 * Find element in list in binary.
 *
 * @param list      A list to search from.
 * @param target    The element to search.
 * @param cmpTimes  If not `NULL`, store the comparison times.
 * @return The index of found element, or `-RERR_NOTFOUND`.
 *
 * @exception RERR_NOTFOUND `target` not found in list.
 */
int ArrayListBinaryFind(const ArrayList *list, NODE_TYPE target, Nullable unsigned *cmpTimes);
#endif
