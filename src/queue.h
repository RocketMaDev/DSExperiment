/**
 * Chapter 3 - STACK & QUEUE
 * Experiment 1.3 - Stack and Queue
 */
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

#ifndef Nullable
#   define Nullable
#endif

/**
 * Initialize the array based queue.
 *
 * @param queue Uninitialized queue.
 */
void QueueInit(Queue *queue);
/**
 * Clear elements in queue.
 *
 * @param queue A queue.
 */
void QueueClear(Queue *queue);
/**
 * Get the size of queue.
 *
 * @param queue A queue.
 * @return The size of it.
 */
unsigned int QueueLength(const Queue *queue);
/**
 * Enqueue an element into queue.
 *
 * @param queue A queue.
 * @param v     The element.
 * @return A `-status` code where `status` is one of member in `RERROR`. `0` if no error.
 *
 * @exception RERR_OVERFLOW If queue is full. To meet textbook, an element is
 *                          left empty to distinguish between full case and empty
 *                          case.
 */
int Enqueue(Queue *queue, NODE_TYPE v);
/**
 * Dequeue an element from queue.
 *
 * @param queue A queue.
 * @param recv  If not `NULL`, store the dequeued element.
 * @return A `-status` code where `status` is one of member in `RERROR`. `0` if no error.
 * 
 * @exception RERR_UNDERFLOW If queue is empty.
 */
int Dequeue(Queue *queue, Nullable NODE_TYPE *recv);
/**
 * Determine if the queue is empty.
 *
 * @param queue A queue.
 * @return If it's empty.
 */
bool QueueEmpty(const Queue *queue);
/**
 * Determine if the queue is full.
 *
 * @param queue A queue.
 * @return If it's full.  To meet textbook, an element is left empty to
 *         distinguish between full case and empty case.
 */
bool QueueFull(const Queue *queue);
#endif
