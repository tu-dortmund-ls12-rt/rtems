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
#include <rtems/rtems/attr.h>

#ifndef CONFIGURE_MAXIMUM_SEGMENTS
#define CONFIGURE_MAXIMUM_SEGMENTS 10
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void (*function) (void);
} Segmented_Task_Segment;

typedef struct {
    /*
    All informations that a normal task needs
    */
   rtems_name taskName;
   rtems_task_priority taskPriority;
   size_t taskStackSize;
   rtems_mode taskModes;
   rtems_attribute taskAttributes;
   rtems_id taskId;

   /*
   Segment specific informations
   */
  uint32_t numberOfSegements;
  Segmented_Task_Segment segments [CONFIGURE_MAXIMUM_SEGMENTS];
  uintptr_t communicationMemory; // TODO: Realization unknown so far. Just so I don't forget.
} Segmented_Task_Task;

#ifdef __cplusplus
}
#endif

#endif