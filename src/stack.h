#ifndef __STACK_H__
#define __STACK_H__

#include "rerror.h"
#include "arraylist.h"
int StackInit(ArrayList *stack);
void StackClear(ArrayList *stack);
unsigned int StackLength(const ArrayList *stack);
int StackPeek(const ArrayList *stack, NODE_TYPE *recv);
int StackPush(ArrayList *stack, NODE_TYPE v);
int StackPop(ArrayList *stack, Nullable NODE_TYPE *recv);
#endif
