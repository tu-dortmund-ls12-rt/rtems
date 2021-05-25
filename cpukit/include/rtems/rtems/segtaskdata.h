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

file1.h -> #define SOMETHING
file2.h -> #ifndef SOMETHING #define SOMETHINGELSE #endif
main.c -> #include file1.h #include file2.h

I expected that in this example case SOMETHING is defined for file2.h, because file1.h defined it
and was included in main.c before. My understanding of #include is like copy-paste of the included file
in the file where it is included. Therefor I expected the preprocessor directive #define is also copy-pasted.
Likewise the #ifndef preprocessor directive would be. Thefore #define and #ifndef would be in the same file
in the end and the compiler would not execute the #ifndef directive because the definition was there.
This doesn't seem to be the case tho. The #define preprocessor directive is only known in the file where this
was written or in files, that include the respective file. It does not transendence file boundarys, even if
both files were included in another file.

This is exactly what happens here. I expected CONFIGURE_MAXIMUM_SEGMENTS to be defined, due to it's definition
in sys_config.h, which is included in main.c before segtaskdata.h (this header file). The above explained scenario
applies tho. Normaly I would fix it with the extra includence #include "sys_config.h", but the problem is that this
header file (segtaskdata.h) is part of the rtems operating system and sys_config.h is part of the application. I'm
unsure how to trandance this border, but it has to be possible, because I can also define informations like
#define CONFIGURE_MAX_TASK_STACK in sys_config.h and this information is accessed inside the rtems operating system.

Need to contact Kuan regarding this...

The current workaround is to just define the configuration again. IMPORTANT: It has to match the informations inside
sys_config.h otherwise the unintend behavior happens.
*/
#define CONFIGURE_MAXIMUM_SEGMENTS 20

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
  uint32_t numberOfSegments;
  Segmented_Task_Segment segments [CONFIGURE_MAXIMUM_SEGMENTS];
  uintptr_t communicationMemory; // TODO: Realization unknown so far. Just so I don't forget.
} Segmented_Task_Task;

#ifdef __cplusplus
}
#endif

#endif