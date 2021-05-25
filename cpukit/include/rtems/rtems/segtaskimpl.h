#ifndef _RTEMS_RTEMS_SEGTASKIMPL_H
#define _RTEMS_RTEMS_SEGTASKIMPL_H

#include <rtems/rtems/attr.h>
#include <rtems/rtems/segtaskdata.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
Zeroes the given Segmented_Task_Task.

Attention: The TaskID is not changed at all, because the segmented
task could have a rtems task assigned with a valid id.
*/
void emptySegmentedTask(Segmented_Task_Task* task);

/*
Fills in all the given data into the given segmentedTask and
zeroes the other informations.

Attention: The TaskID is not changed at all, because the segmented
task could have a rtems task assigned with a valid id.
*/
void fillDataIntoSegTask(Segmented_Task_Task* task,
                rtems_name taskName, rtems_task_priority taskPriority,
                size_t taskStackSize, rtems_mode initialModes, 
                rtems_attribute taskAttributes, uint32_t numberOfSegments,
                void (*functionPointer[]) (void));

/*
Fills in all the general given data into the given segmentedTask.

Attention: The TaskID is not changed at all, because the segmented
task could have a rtems task assigned with a valid id.
*/
void fillGeneralDataIntoSegTask(Segmented_Task_Task* task,
                rtems_name taskName, rtems_task_priority taskPriority,
                size_t taskStackSize, rtems_mode initialModes, 
                rtems_attribute taskAttributes);

/*
Fills in all the segment specific given data into the given segmentedTask.
*/
void fillSegmentDataIntoSegTask(Segmented_Task_Task* task,
                uint32_t numberOfSegments, void (*functionPointer[]) (void));

#ifdef __cplusplus
}
#endif

#endif