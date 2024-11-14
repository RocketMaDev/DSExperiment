#include "qsoperation.h"
#define IFERR(stat) \
    if ((err = stat)) \
        return err

int Conversion(unsigned int dec, unsigned int tobase, ArrayList *stack) {
    int err;
    if (tobase == 2)
        while (dec > 0) {
            IFERR(ArrayListAppend(stack, dec & 1));
            dec >>= 1;
        }
    else if (tobase == 8)
        while (dec > 0) {
            IFERR(ArrayListAppend(stack, dec & 7));
            dec >>= 3;
        }
    else 
        return -RERR_NOTIMPLEMENTED;
    return -RERR_OK;
}

int QueueReverse(Queue *queue) {
    if (QueueEmpty(queue))
        return -RERR_OK;
    ArrayList buf;
    int err;
    IFERR(ArrayListInit(&buf, QueueLength(queue)));
    while (!QueueEmpty(queue)) {
        NODE_TYPE recv;
        Dequeue(queue, &recv);
        StackPush(&buf, recv); // not possible to overflow
    }
    while (ArrayListSize(&buf)) {
        NODE_TYPE recv;
        StackPop(&buf, &recv);
        Enqueue(queue, recv); // not possible to overflow
    }
    return -RERR_OK;
}
#undef IFERR
