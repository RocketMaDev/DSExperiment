/**
 * Chapter 2 - Linear Table
 * Experiment 1.2 - Linked Linear Table
 */
#ifndef __LIST_OPERATION_H__
#define __LIST_OPERATION_H__

#include "rerror.h"
#include "linkedlist.h"
/**
 * Treat `a` and `b` as sets and perform a union operation on `a` and `b`.
 *
 * @param a     A list.
 * @param b     Another list.
 * @param ans   The list to store result. If `NULL`, store result in `a`.
 *
 * @exception RERR_OOM Insufficient memory when running.
 */
int Union(LinkedList *a, const LinkedList *b, Nullable LinkedList *ans);
/**
 * Treat `a` and `b` as sets and perform a intersection operation on `a` and `b`.
 *
 * @param a     A list.
 * @param b     Another list.
 * @param ans   The list to store result. If `NULL`, store result in `a`.
 *
 * @exception RERR_OOM Insufficient memory when running.
 */
int Intersection(LinkedList *a, const LinkedList *b, Nullable LinkedList *ans);
/**
 * Treat `a` and `b` as sets and perform a difference operation on `a` and `b`.
 *
 * @param a     A list.
 * @param b     Another list.
 * @param ans   The list to store result. If `NULL`, store result in `a`.
 *
 * @exception RERR_OOM Insufficient memory when running.
 */
int Difference(LinkedList *a, const LinkedList *b, Nullable LinkedList *ans);

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
int MergeList(LinkedList *a, const LinkedList *b, Nullable LinkedList *res);

/**
 * Remove duplicate elements in `list`.
 *
 * @param list The list to purge. Note that `arr` in list will vary.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_OOM Insufficient memory when running.
 */
int Purge(LinkedList *list);
#endif
