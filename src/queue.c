#include "queue.h"
void QueueInit(Queue *queue) {
    queue->front = 0;
    queue->rear = 0;
}

void QueueClear(Queue *queue) {
    QueueInit(queue);
}

unsigned int QueueLength(const Queue *queue) {
    return (queue->rear - queue->front + QUEUE_SIZE) % QUEUE_SIZE;
}

int Enqueue(Queue *queue, NODE_TYPE v) {
    if (QueueLength(queue) == QUEUE_SIZE - 1)
        return -RERR_OVERFLOW;
    queue->arr[queue->rear] = v;
    queue->rear = (queue->rear + 1) % QUEUE_SIZE;
    return -RERR_OK;
}

int Dequeue(Queue *queue, Nullable NODE_TYPE *recv) {
    if (QueueEmpty(queue))
        return -RERR_UNDERFLOW;
    if (recv)
        *recv = queue->arr[queue->front];
    queue->front = (queue->front + 1) % QUEUE_SIZE;
    return -RERR_OK;
}

bool QueueEmpty(const Queue *queue) {
    return queue->front == queue->rear;
}

bool QueueFull(const Queue *queue) {
    return QueueLength(queue) == QUEUE_SIZE - 1;
}

