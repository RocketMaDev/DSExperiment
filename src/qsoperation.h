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
/**
 * Convert a decimal number to a list of numbers with specified base, and
 * put them in `list`. For example, `Conversion(6, 2, list)` will produce
 * `[0, 1, 1]`. According to the experiment tasks, only bin and oct is implemented.
 * Though it's easy to implement all bases.
 *
 * @param dec       A unsigned decimal.
 * @param tobase    Target base.
 * @param list      The list to store conversion result.
 * @return A `-status` code where `status` is one of member in `RERROR`. `0` if no error.
 *
 * @exception RERR_NOTIMPLEMENTED   `tobase` is not 2 or 8.
 * @exception RERR_OOM              Insufficient memory when expanding list.
 */
int Conversion(unsigned int dec, unsigned int tobase, ArrayList *list);
/**
 * Use stack to reverse the queue. The `front` and `rear` of queue will vary.
 *
 * @param queue The queue.
 * @return A `-status` code where `status` is one of member in `RERROR`. `0` if no error.
 *
 * @exception RERR_OOM  Insufficient memory when running.
 */
int QueueReverse(Queue *queue);
#endif
