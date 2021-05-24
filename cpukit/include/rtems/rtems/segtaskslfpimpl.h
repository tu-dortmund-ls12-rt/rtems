#ifndef _RTEMS_RTEMS_SEGTASKSLFPIMPL_H
#define _RTEMS_RTEMS_SEGTASKSFLPIMPL_H

#include <rtems/rtems/segtaskslfpdata.h>
#include <rtems/rtems/attr.h>

#ifdef __cplusplus
extern "C" {
#endif

bool fillAssociatedDataStructureGeneral(Segmented_Task_Task* task,
    rtems_name taskName, rtems_task_priority taskPriority,
                size_t taskStackSize, rtems_mode initialModes, 
                rtems_attribute taskAttributes);

bool fillAssociatedDataStructure(Segmented_Task_SLFP_Task* task,
    rtems_name taskName, rtems_task_priority taskPriority,
                size_t taskStackSize, rtems_mode initialModes, 
                rtems_attribute taskAttributes);

#ifdef __cplusplus
}
#endif

#endif