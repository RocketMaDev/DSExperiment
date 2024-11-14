#include "stack.h"
#include "arraylist.h"
int StackInit(ArrayList *stack) {
    return ArrayListInit(stack, 0);
}

void StackClear(ArrayList *stack) {
    ArrayListClear(stack);
}

unsigned int StackLength(const ArrayList *stack) {
    return ArrayListSize(stack);
}

int StackPeek(const ArrayList *stack, NODE_TYPE *recv) {
    if (stack->size == 0)
        return -RERR_UNDERFLOW;
    *recv = stack->arr[stack->size - 1];
    return -RERR_OK;
}

int StackPush(ArrayList *stack, NODE_TYPE v) {
    return ArrayListAppend(stack, v);
}

int StackPop(ArrayList *stack, Nullable NODE_TYPE *recv) {
    if (stack->size == 0)
        return -RERR_UNDERFLOW;
    ArrayListDelete(stack, stack->size - 1, recv);
    return -RERR_OK;
}

