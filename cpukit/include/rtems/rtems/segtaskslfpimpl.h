#ifndef _RTEMS_RTEMS_SEGTASKSLFPIMPL_H
#define _RTEMS_RTEMS_SEGTASKSFLPIMPL_H

#include <rtems/rtems/segtaskimpl.h>
#include <rtems/rtems/segtaskslfpdata.h>
#include <rtems/rtems/attr.h>

#ifdef __cplusplus
extern "C" {
#endif

void fillDataIntoSegTaskSLFP(Segmented_Task_SLFP_Task* task,
                rtems_name taskName, size_t taskStackSize,
                rtems_mode initialModes, rtems_attribute taskAttributes,
                uint32_t numberOfSegments, void (*functionPointer[]) (void),
                rtems_task_priority priorities[]);

#ifdef __cplusplus
}
#endif

#endif