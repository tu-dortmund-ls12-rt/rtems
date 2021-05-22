/*
A segmented self suspending  task is expected to have a least one segment.
Each segment of the self suspending task is expected to have a self suspending interval
afterwards, except for the last segment. Therefor the task starts and ends with a segment
with suspending intervalls inbetween. Neither the worst case execution time of the segments
nor the "wcet" of the suspension intervalls is known. Regarding the self suspension intervalls,
a task can only be unsuspended by an external call.
*/

#ifndef _RTEMS_RTEMS_SEGTASKDATA_H
#define _RTEMS_RTEMS_SEGTASKDATA_H

#include <rtems/rtems/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    /*
    Task name   -> Be conform with rtems naming
    Priority of the task (?)    -> Can be overwritten acording to the used scheduling.
    Number of segments  ->  Needed for the initialization of the array afterwards
    Functions for the segments  -> So the functions can execute independently
    Memory for communication -> Needed to pass data from one segment to the next one
    */

    /*
    TODO:
    Problem: Arrays size is determined at runtime. Therefor the struct needs to be allocated
    dynamicly and freed afterwards. How is it done it RTEMS? We have to pay attention to differ
    the allocation on the heap of the actual application and the alocation on the heap of the os.
    Or is there a difference? :s Not sure! (Question is regarding to if we can simply use the
    _HEAP_Allocation (or similiar) methods or if they are dedicated to use only in the actual
    application and not in the core of rtems)
    */

    rtems_name name;
    rtems_task_priority priority;
    uint32_t numberOfSegments;
    uintptr_t* communcationMemory;
    uintptr_t* test[];
} Segmented_Task_Task;

#ifdef __cplusplus
}
#endif

#endif