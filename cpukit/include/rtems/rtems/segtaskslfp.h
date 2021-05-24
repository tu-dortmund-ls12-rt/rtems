#ifndef _RTEMS_RTEMS_SEGTASKSLFP_H
#define _RTEMS_RTEMS_SEGTASKSFLP_H

#include <rtems/rtems/segtaskslfpdata.h>

#include <rtems.h>
#include <rtems/rtems/status.h>

#ifdef __cplusplus
extern "C" {
#endif

rtems_status_code rtems_task_create_segmented_slfp(rtems_name taskName,
                rtems_task_priority taskPriority, size_t taskStackSize,
                rtems_mode initialModes, rtems_attribute taskAttributes,
                uint32_t numberOfSegments, void (*segmentFunctions[]) (void),
                rtems_task_priority segmentPriorities[], Segmented_Task_SLFP_Task* taskCopy);

#ifdef __cplusplus
}
#endif

#endif