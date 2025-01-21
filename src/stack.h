#ifndef __STACK_H__
#define __STACK_H__

#include "rerror.h"
#include "arraylist.h"
/**
 * Initialize an array list based stack.
 *
 * @param stack A uninitialized stack.
 * @see `ArrayListInit`
 */
int StackInit(ArrayList *stack);
/**
 * Clear a stack.
 *
 * @param stack A stack.
 * @see `ArrayListClear`
 */
void StackClear(ArrayList *stack);
/**
 * Get the length of the stack.
 *
 * @param stack A stack.
 * @see `ArrayListSize`
 */
unsigned int StackLength(const ArrayList *stack);
/**
 * Peek the stack top.
 *
 * @param stack A stack.
 * @param recv  A non-null pointer to store element.
 * @see `ArrayListGet`
 */
int StackPeek(const ArrayList *stack, NODE_TYPE *recv);
/**
 * Push an element into stack.
 *
 * @param stack A stack.
 * @param v     The element to push.
 * @see `ArrayListAppend`
 */
int StackPush(ArrayList *stack, NODE_TYPE v);
/**
 * Pop an element from stack.
 * 
 * @param stack A stack.
 * @param recv  If not `NULL`, store stack top in it.
 * @see `ArrayListDelete`
 */
int StackPop(ArrayList *stack, Nullable NODE_TYPE *recv);
#endif
