#ifndef _RTEMS_RTEMS_SEGTASK_H
#define _RTEMS_RTEMS_SEGTASK_H

#include <rtems/rtems/segtaskslfp.h>

#define DO_ZERO_ON_WRITE 0

/*
Returns the size of the allocated memory for communcation between segments.

TODO:
So far each segmented task has the same sized communication memory. The result is therefor indepented of the given
segmented task.
*/
rtems_status_code rtems_task_segmented_get_communication_memory_size(Segmented_Task_Task* segmentedTask, size_t* size);

/*
Writes to the beginning of the allocated communication memory of the given segmented task.
Boundary checks are performed, so therefor it is not possible to write out of the intended
memory if the correct size in bytes of the content is given.
If the communication memory is fully zeroed beforehand depends on DO_ZERO_ON_WRITE.
*/
rtems_status_code rtems_task_segmented_write_communication_memory(Segmented_Task_Task* segmentedTask, uint8_t* content, size_t sizeOfContent);

/*
Appends the given content after the last written content in the communication memory.
Boundary checks are performed, so therefor it is not possible to write out of the intended
memory if the correct size in bytes of the content is given.
*/
rtems_status_code rtems_task_segmented_append_communication_memory(Segmented_Task_Task* segmentedTask, uint8_t* content, size_t sizeOfContent);

/*
"Empties" the communication memory of the segmented task. This means it's whole content is zeroed.
This also resets the nextByteToWrite information back to 0.
*/
rtems_status_code rtems_task_segmented_clear_communication_memory(Segmented_Task_Task* segmentedTask);

#endif