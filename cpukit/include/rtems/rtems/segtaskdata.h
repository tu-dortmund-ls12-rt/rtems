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

/*
TODO:
WORKAROUND!

Example case:
file1.h
file2.h
main.c
second.c

file1.h -> #define SOMETHING
file2.h -> #ifndef SOMETHING #define SOMETHINGELSE #endif
main.c -> #include file1.h #include file2.h
second.c -> #include file2.h

My expectation for the example case was the following: A datastructure that was defined in
file2.h is dependet on the definition of SOMETHING or SOMETHINGELSE (depending on if SOMETHING
was defined). So therefor the actual datastructure, which is used in main.c and second.c, is the
same, depending on the informations (if defined or not) present in file2.h. I thought SOMETHING was
defined in file2.h and therefor dictates the actual datastructure in main.c and second.c, because
it is defined in file1.h which was included first in main.c and afterwards file2.h is included in main.c.
My thinking was: Well the definition was included first, so it should be present in the second includence.
And this is actually true (which is also denoted in the IDE when the defintion is inspected), but this is
only true for main.c, file1.h and file2.h itself! For second.c file2.h was included, but file1.h was never.
Therefor, from the viewpoint of second.c, SOMETHING was never defined and SOMETHINGELSE is therefor definend
by file2.h. I got tricked here a little by the support of the IDE.

This example case is exactly what is happening for the datastructure Segmented_Task_Task in segtaskdata.h.
I thought I defined CONFIGURE_MAXIMUM_SEGMENTS in sys_config.h which is (indirectly) included in main.c, which
also includes segtaskdata.h. Then I wondered why the datastructure was of different size in segtaskslfp.c. Well,
it's size is dependent on the definition, due to it's determining the size of an array, and from the viewpoint of
segtaskslfp.c, which only includes segtaskdata.h, not sys_config.h, CONFIGURE_MAXIMUM_SEGMENTS was never defined.

To solve this problem I normaly would just include sys_config.h in segtaskdata.h, so it is always certain for each
.c file if CONFIGURE_MAXIMUM_SEGMENTS was defined or not. But this is not possible in this case, because sys_config.h
is part of the application files and segtaskdata.h is part of the RTEMS operating system files. So therefor it can
not be included. But it has to be possible to access those application defined informations from the operating
system, because the same is done with for example #define CONFIGURE_MAX_TASK_STACK in sys_config.h and this information
is accessed inside the rtems operating system.

Need to contact Kuan regarding this...

The current workaround is to just define the configuration again. IMPORTANT: It has to match the informations inside
sys_config.h otherwise the unintend behavior happens. HUGE SOURCE OF ERRORS HERE!
*/
#define CONFIGURE_MAXIMUM_SEGMENTS 20

#ifndef CONFIGURE_MAXIMUM_SEGMENTS
#define CONFIGURE_MAXIMUM_SEGMENTS 10
#endif

/*
TODO:
WORKAROUND!

Same problem as described for CONFIGURE_MAXIMUM_SEGMENTS does also apply for CONFIGURE_MAXIMUM_COMMUNCATION_MEMORY.
*/
#define CONFIGURE_MAXIMUM_COMMUNICATION_MEMORY 256

#ifndef CONFIGURE_MAXIMUM_COMMUNICATION_MEMORY
#define CONFIGURE_MAXIMUM_COMMUNICATION_MEMORY 64
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
  uint32_t numberOfSegments;
  Segmented_Task_Segment segments[CONFIGURE_MAXIMUM_SEGMENTS];
  uint8_t communicationMemory[CONFIGURE_MAXIMUM_COMMUNICATION_MEMORY];
  uint32_t nextByteToWrite;
} Segmented_Task_Task;

#ifdef __cplusplus
}
#endif

#endif