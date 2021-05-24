#ifndef _RTEMS_RTEMS_SEGTASKIMPL_H
#define _RTEMS_RTEMS_SEGTASKIMPL_H

#include <rtems/rtems/segtaskslfpdata.h>
#include <rtems/rtems/attr.h>

#ifdef __cplusplus
extern "C" {
#endif

void fillDataIntoSegTask(Segmented_Task_Task* task,
                rtems_name taskName, rtems_task_priority taskPriority,
                size_t taskStackSize, rtems_mode initialModes, 
                rtems_attribute taskAttributes, uint32_t numberOfSegments,
                void (*functionPointer[]) (void));

void fillGeneralDataIntoSegTask(Segmented_Task_Task* task,
                rtems_name taskName, rtems_task_priority taskPriority,
                size_t taskStackSize, rtems_mode initialModes, 
                rtems_attribute taskAttributes);

void fillSegmentDataIntoSegTask(Segmented_Task_Task* task,
                uint32_t numberOfSegments, void (*functionPointer[]) (void));

#ifdef __cplusplus
}
#endif

#endif