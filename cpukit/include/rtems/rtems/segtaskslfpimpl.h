#ifndef _RTEMS_RTEMS_SEGTASKSLFPIMPL_H
#define _RTEMS_RTEMS_SEGTASKSLFPIMPL_H

#include <rtems/rtems/segtaskimpl.h>
#include <rtems/rtems/segtaskslfpdata.h>
#include <rtems/rtems/attr.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
Info regarding "Segmented_Task_SLFP_Task segmentedTask;":

The declaration of this variable was originaly in this header file. Cause this used
to be the way in C++. But the problem in this case is, that segtaskslfpimpl.h includes
segtaskimpl.h and vice versa. Therefor, if the variable is declared in the header
file, then there are multiple declarations of the same variable. It is therefor
moved in the *.c file. If it's "somehow" needed in this header file or in another
file, it needs to be declared as "extern".

TODO:
Is there a disadvantage declaring it in the *.c file? It'll be still allocated on the
heap of the RTEMS OS right? Think this through, because u don't want to let this
variable get out of scope ever!
*/

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