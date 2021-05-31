#ifndef _RTEMS_RTEMS_SEGTASKSLFP_H
#define _RTEMS_RTEMS_SEGTASKSLFP_H

#include <rtems/rtems/segtaskslfpdata.h>

#include <rtems.h>
#include <rtems/rtems/status.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
Create a segmented task with the given informaitons that will be scheduled according
to SLFP.

TODO:
Remove the possibility to receive a copy of the created datastructure. This is only
for debugging purpose.
*/
rtems_status_code rtems_task_create_segmented_slfp(rtems_name taskName,
                rtems_task_priority taskPriority, size_t taskStackSize,
                rtems_mode initialModes, rtems_attribute taskAttributes,
                uint32_t numberOfSegments, void (*segmentFunctions[]) (void),
                rtems_task_priority segmentPriorities[], rtems_id* taskId);

/*
Start the segmented task with the given id accoring to SLFP scheduling.
If no segmented task that can be scheduled with slfp according to the id
exists, an error will be returned.

TODO:
Currently can only one segmentedTask exists. So therefor the given id will have
effect.
*/
rtems_status_code rtems_task_start_segmented_slfp(rtems_id taskId);

#ifdef __cplusplus
}
#endif

#endif