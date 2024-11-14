#ifndef __QS_OPERATION_H__
#define __QS_OPERATION_H__
#ifdef NODE_TYPE
#   undef NODE_TYPE
#endif
#define NODE_TYPE int
#include "rerror.h"
#include "arraylist.h"
#include "stack.h"
#include "queue.h"
int Conversion(unsigned int dec, unsigned int tobase, ArrayList *list);
int QueueReverse(Queue *queue);
#endif
