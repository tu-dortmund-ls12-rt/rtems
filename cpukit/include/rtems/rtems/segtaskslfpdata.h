#ifndef _RTEMS_RTEMS_SEGTASKSLFPDATA_H
#define _RTEMS_RTEMS_SEGTASKSLFPDATA_H

#include <rtems/rtems/segtaskdata.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
Structure for a segmented task that shall be scheduled according to segmented level
fixed priority scheduling. Informations in addition to the informations contained in
a general segmented task are the priorities for each segment.

TODO: How to ensure there are only as much priorities as segments? Possible without
addition variable information.
*/
typedef struct {
    Segmented_Task_Task base;
    rtems_task_priority priorities[CONFIGURE_MAXIMUM_SEGMENTS];
} Segmented_Task_SLFP_Task;

#ifdef __cplusplus
}
#endif

#endif