#ifndef _RTEMS_RTEMS_SEGTASKSLFPIMPL_H
#define _RTEMS_RTEMS_SEGTASKSFLPIMPL_H

#include <rtems/rtems/segtaskimpl.h>
#include <rtems/rtems/segtaskslfpdata.h>
#include <rtems/rtems/attr.h>

#ifdef __cplusplus
extern "C" {
#endif

Segmented_Task_SLFP_Task segmentedTask; // Currently only one segmented task is possible. Should be replaced with a dynamic data structure.

bool getSegmented_Task_SLFP_Task(rtems_id id, Segmented_Task_SLFP_Task** segmentedTaskToReturn);

/*
"Empties" the segmented slfp task. "Empties" mean zeroing.

TODO:
Currently the given segmentedTask has no effect, because there is always only
segmentedTask currently possible. So therefor it won't be used. This needs
to be changed when a list of Segmented_TASK_SLFP_Tasks is present.
*/
void emptySegTaskSLFP(Segmented_Task_SLFP_Task* givenSegmentedTask);

/*
Fills in all the given data into the given segmentedTask and
zeroes the other informations.

Attention: The TaskID is not changed at all, because the segmented
task could have a rtems task assigned with a valid id.
*/
void fillDataIntoSegTaskSLFP(Segmented_Task_SLFP_Task* task,
                rtems_name taskName, size_t taskStackSize,
                rtems_mode initialModes, rtems_attribute taskAttributes,
                uint32_t numberOfSegments, void (*functionPointer[]) (void),
                rtems_task_priority priorities[]);

/*
This is the main function which executes the segments,
performs the priority switching and the selfsuspension.

TODO:
Rename to avoid compiler warnings.
Implement loop for multiple execution of a segmented task.
*/
void main(rtems_task_argument arguments);

#ifdef __cplusplus
}
#endif

#endif