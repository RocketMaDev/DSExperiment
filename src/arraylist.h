/**
 * Chapter 2 - LINEAR TABLE
 * Experiment 1.1 - Sequential Linear Table
 */
#ifndef __ARRAY_LIST_H__
#define __ARRAY_LIST_H__
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

typedef struct {
    NODE_TYPE *arr;
    unsigned int size;
    unsigned int capacity;
} ArrayList;

/**
 * Init a arraylist with given length
 *
 * @param list      An uninitialized arraylist.
 * @param initSize  The initial capacity for the dynamic array. If less than 16, `capacity`
 *                  will still set to 16.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_OOM      Insufficient memory when creating the list.
 * @exception RERR_OVERFLOW `initSize` is larger than `MAX_SIZE`   
 */
int ArrayListInit(ArrayList *list, unsigned int initSize);
/**
 * Clear a arraylist, remove all elements in it. This function will not free the array.
 *
 * @param list  An arraylist.
 */
void ArrayListClear(ArrayList *list);
/**
 * Clear the arraylist and free the list. Call it when you will want to "delete" a list.
 *
 * @param list  An arraylist.
 */
void ArrayListRelease(ArrayList *list);

/**
 * Append an element to the list. If `capacity` is not enough, try to expand the dynamic
 * array with `realloc`. The `capacity` will automatically expand by adding roughly
 * half of the original `capacity`.
 *
 * @param list  An arraylist.
 * @param v     The element to append.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_OOM      Insufficient memory when expanding array.
 * @exception RERR_OVERFLOW `size` has reached `MAX_SIZE` and can not add any new element.
 */
int ArrayListAppend(ArrayList *list, NODE_TYPE v);
/**
 * Insert to an element to the list. If `index` is `0`, equivalent to **prepend**,
 * if `index` is `size`, equivalent to **append**.
 *
 * @param list  An arraylist.
 * @param index Where to insert the element, push elements from there to back.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_OOM Insufficient memory when expanding array.
 * @exception RERR_OVERFLOW `size` has reached `MAX_SIZE` and can not add any new element.
 */
int ArrayListInsert(ArrayList *list, unsigned int index, NODE_TYPE v);
/**
 * Prepend an element to the list. Note that this operation may involves massive moves.
 *
 * @param v     The element to append.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_OOM      Insufficient memory when expanding array.
 * @exception RERR_OVERFLOW `size` has reached `MAX_SIZE` and can not add any new element.
 */ 
int ArrayListPrepend(ArrayList *list, NODE_TYPE v);
/**
 * Delete an element from list with given index.
 *
 * @param list  An arraylist.
 * @param index Where to delete the element. Following elements will fill up the space.
 * @param recv  A pointer to receive the element to delete. If you don't need it,
 *              set it to `NULL`.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_OVERFLOW `index >= size`, element is not exist.
 */
int ArrayListDelete(ArrayList *list, unsigned int index, Nullable NODE_TYPE *recv);

/**
 * Update an element at index in the arraylist.
 *
 * @param list  An arraylist.
 * @param index Which element to update.
 * @param v     The new element.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_OVERFLOW `index >= size`, element is not exist.
 */
int ArrayListSet(ArrayList *list, unsigned int index, NODE_TYPE v);
/**
 * Get an element at index int the arraylist.
 *
 * @param list  An arraylist.
 * @param index Which element to get.
 * @param recv  A pointer to receive the element. Nullable.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 *
 * @exception RERR_OVERFLOW `index >= size`, element is not exist.
 */
int ArrayListGet(const ArrayList *list, unsigned int index, Nullable NODE_TYPE *recv);

/**
 * Get the size of arraylist.
 *
 * @param list An arraylist.
 * @return The size of the list. (not capacity)
 */
unsigned int ArrayListSize(const ArrayList *list);

/**
 * Find an element in the list with linear scan.
 *
 * @param list  An arraylist.
 * @param v     The element to find.
 * @return The index of left most found element, or `-RERR_NOTFOUND`.
 *
 * @exception RERR_NOTFOUND Identical element not found in list.
 */
int ArrayListFind(const ArrayList *list, NODE_TYPE v);
/**
 * Remove elements which equal to `v`.
 *
 * @param list  An arraylist.
 * @param v     The element to find.
 * @return The count of removed elements if `v` found, or else `-RERR_NOTFOUND`.
 *
 * @exception RERR_NOTFOUND `v` not found in list.
 */
int ArrayListRemove(ArrayList *list, NODE_TYPE v);

/**
 * Traverse every element in list with `func`.
 *
 * @param list  An arraylist.
 * @param buf   An arbitrary pointer which will be `func`'s third arg when called.
 * @param func  The function to traverse the list.
 */
void ArrayListTraverse(ArrayList *list, void *buf, void (*func)(unsigned int index, NODE_TYPE v, void *buf));
/**
 * Extend `dst` with `src`. If `dst` is empty, then it acts like clone.
 *
 * @param dst   The list to be extended.
 * @param src   The source list to extend `dst`.
 * @return A `-status` code where `status` is one of members in `RERROR`. `0` if no error.
 * 
 * @exception RERR_OVERFLOW If size of extended `dst` exceeds `MAX_SIZE`. `dst` left unchanged.
 * @exception RERR_OOM Insufficient memory when expanding array.
 */
int ArrayListExtend(ArrayList *dst, const ArrayList *src);
#endif
