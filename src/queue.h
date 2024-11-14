#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdbool.h>
#include "rerror.h"
#ifndef QUEUE_SIZE
#   define QUEUE_SIZE 16
#endif

#ifndef NODE_TYPE
#   define NODE_TYPE int
#endif

typedef struct {
    NODE_TYPE arr[QUEUE_SIZE];
    unsigned int front;
    unsigned int rear;
} Queue;

enum {
    QERR_OK = 0,
    QERR_OVERFLOW = 1,
    QERR_UNDERFLOW = 2,
};

#ifndef Nullable
#   define Nullable
#endif

void QueueInit(Queue *queue);
void QueueClear(Queue *queue);
unsigned int QueueLength(const Queue *queue);
int Enqueue(Queue *queue, NODE_TYPE v);
int Dequeue(Queue *queue, Nullable NODE_TYPE *recv);
bool QueueEmpty(const Queue *queue);
bool QueueFull(const Queue *queue);
#endif
