/**
 * Chapter 2 - Linear Table
 * Experiment 1.2 - Linked Linear Table
 */
#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__
#include "rerror.h"
#ifndef NODE_TYPE
#   define NODE_TYPE int
#endif

#ifndef MAX_SIZE
#   define MAX_SIZE 1000
#endif

#define Nullable

#   ifdef OVERRIDE_CMP
typedef int (*NODE_HOOK)(NODE_TYPE a, NODE_TYPE b);
extern NODE_HOOK NodeCmp;
#   endif

typedef struct __node {
    NODE_TYPE value;
    struct __node *next;
} NODE;

typedef struct {
    NODE *head;
    NODE *tail;
    unsigned int size;
} LinkedList;

/**
 * Init a linked list with given length
 *
 * @param list  An uninitialized linked list.
 */
void LinkedListInit(LinkedList *list);
/**
 * Clear a linked list, remove all elements in it. For linked list,
 * this is equivalent to "release".
 *
 * @param list  An linked list.
 */
void LinkedListClear(LinkedList *list);
/**
 * Clear the linked list and free elements in the list.
 *
 * @param list  An linked list.
 */
void LinkedListRelease(LinkedList *list);

/**
 * Append an element to the list.
 *
 * @param list  An linked list.
 * @param v     The element to append.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_OOM      Insufficient memory when expanding list.
 * @exception RERR_OVERFLOW `size` has reached `MAX_SIZE` and can not add any new element.
 */
int LinkedListAppend(LinkedList *list, NODE_TYPE v);
/**
 * Insert to an element to the list. If `index` is `0`, equivalent to **prepend**,
 * if `index` is `size`, equivalent to **append**.
 *
 * @param list  An linked list.
 * @param index Where to insert the element, push elements from there to back.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_OOM Insufficient memory when expanding list.
 * @exception RERR_OVERFLOW `size` has reached `MAX_SIZE` and can not add any new element.
 */
int LinkedListInsert(LinkedList *list, unsigned int index, NODE_TYPE v);
/**
 * Prepend an element to the list.
 *
 * @param v     The element to append.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_OOM      Insufficient memory when expanding list.
 * @exception RERR_OVERFLOW `size` has reached `MAX_SIZE` and can not add any new element.
 */ 
int LinkedListPrepend(LinkedList *list, NODE_TYPE v);
/**
 * Delete an element from list with given index.
 *
 * @param list  An linked list.
 * @param index Where to delete the element. Following elements will fill up the space.
 * @param recv  A pointer to receive the element to delete. If you don't need it,
 *              set it to `NULL`.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_OVERFLOW `index >= size`, element is not exist.
 */
int LinkedListDelete(LinkedList *list, unsigned int index, Nullable NODE_TYPE *recv);

/**
 * Update an element at index in the linked list.
 *
 * @param list  An linked list.
 * @param index Which element to update.
 * @param v     The new element.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_OVERFLOW `index >= size`, element is not exist.
 */
int LinkedListSet(LinkedList *list, unsigned int index, NODE_TYPE v);
/**
 * Get an element at index int the linked list.
 *
 * @param list  An linked list.
 * @param index Which element to get.
 * @param recv  A pointer to receive the element. Nullable.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_OVERFLOW `index >= size`, element is not exist.
 */
int LinkedListGet(const LinkedList *list, unsigned int index, Nullable NODE_TYPE *recv);

/**
 * Get the size of linked list.
 *
 * @param list An linked list.
 * @return The size of the list.
 */
unsigned int LinkedListSize(const LinkedList *list);

/**
 * Find an element in the list with linear scan.
 *
 * @param list  An linked list.
 * @param v     The element to find.
 * @return The index of left most found element, or `-RERR_NOTFOUND`.
 *
 * @exception RERR_NOTFOUND Identical element not found in list.
 */
int LinkedListFind(const LinkedList *list, NODE_TYPE v);
/**
 * Remove elements which equal to `v`.
 *
 * @param list  An linked list.
 * @param v     The element to find.
 * @return The count of removed elements if `v` found, or else `-RERR_NOTFOUND`.
 *
 * @exception RERR_NOTFOUND `v` not found in list.
 */
int LinkedListRemove(LinkedList *list, NODE_TYPE v);

/**
 * Traverse every element in list with `func`.
 *
 * @param list  An linked list.
 * @param func  The function to traverse the list.
 */
void LinkedListTraverse(LinkedList *list, void (*func)(unsigned int index, NODE_TYPE v));
/**
 * Extend `dst` with `src`. If `dst` is empty, then it acts like clone.
 *
 * @param dst   The list to be extended.
 * @param src   The source list to extend `dst`.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 * 
 * @exception RERR_OVERFLOW If size of extended `dst` exceeds `MAX_SIZE`. `dst` left unchanged.
 * @exception RERR_OOM Insufficient memory when expanding list.
 */
int LinkedListExtend(LinkedList *dst, const LinkedList *src);
#endif
