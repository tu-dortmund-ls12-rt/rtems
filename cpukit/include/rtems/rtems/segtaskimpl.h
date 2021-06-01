#ifndef _RTEMS_RTEMS_SEGTASKIMPL_H
#define _RTEMS_RTEMS_SEGTASKIMPL_H

#include <rtems/rtems/attr.h>
#include <rtems/rtems/segtaskdata.h>
#include <rtems/rtems/segtaskslfpimpl.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DO_ZERO_ON_WRITE 0

/*
Executes the function of the next segment of the given segmented task.
*/
void executeNextSegment(Segmented_Task_Task* task);

/*
Zeroes the given Segmented_Task_Task.

Attention: The TaskID is not changed at all, because the segmented
task could have a rtems task assigned with a valid id.
*/
void emptySegmentedTask(Segmented_Task_Task* task);

bool getSegmented_Task_Task(rtems_id id, Segmented_Task_Task** segmentedTaskToReturn);

/*
Fills in all the given data into the given segmentedTask and
zeroes the other informations.

Attention: The TaskID is not changed at all, because the segmented
task could have a rtems task assigned with a valid id.
*/
void fillDataIntoSegTask(Segmented_Task_Task* task,
                rtems_name taskName, rtems_task_priority taskPriority,
                size_t taskStackSize, rtems_mode initialModes, 
                rtems_attribute taskAttributes, uint32_t numberOfSegments,
                void (*functionPointer[]) (void));

/*
Fills in all the general given data into the given segmentedTask.

Attention: The TaskID is not changed at all, because the segmented
task could have a rtems task assigned with a valid id.
*/
void fillGeneralDataIntoSegTask(Segmented_Task_Task* task,
                rtems_name taskName, rtems_task_priority taskPriority,
                size_t taskStackSize, rtems_mode initialModes, 
                rtems_attribute taskAttributes);

/*
Fills in all the segment specific given data into the given segmentedTask.
*/
void fillSegmentDataIntoSegTask(Segmented_Task_Task* task,
                uint32_t numberOfSegments, void (*functionPointer[]) (void));

/*
Returns the size of the allocated memory for communcation between segments.

TODO:
So far each segmented task has the same sized communication memory. The result is therefor indepented of the given
segmented task.
*/
rtems_status_code rtems_task_segmented_get_communication_memory_size_impl(Segmented_Task_Task* segmentedTask, size_t* size);

/*
Read the communication memory of the given segmented task for the given number of bytes
starting at a specific byte (inclusive) into the given buffer.
Boundary checks are performed, so therefor it is not possible to read out of the intended
memory.
A copy of the memory content is returned, so it's not possible to manipulate the memory.
*/
rtems_status_code rtems_task_segmented_read_communication_memory_for_from_impl(Segmented_Task_Task* segmentedTask, uint8_t* buffer, size_t bufferSize,
                                                                        size_t amount, uint32_t startingAtByte);

/*
Read the communication memory of the given segmented task for the given number of bytes
starting at the beginning of the memory into the given buffer.
Boundary checks are performed, so therefor it is not possible to read out of the intended
memory.
A copy of the memory content is returned, so it's not possible to manipulate the memory.
*/
rtems_status_code rtems_task_segmented_read_communication_memory_for_impl(Segmented_Task_Task* segmentedTask, uint8_t* buffer, size_t bufferSize,
                                                                        size_t amount);

/*
Read the whole communication memory of the given segmented task starting at a specific
byte (inclusive) into the given buffer.
Boundary checks are performed, so therefor it is not possible to read out of the intended
memory.
A copy of the memory content is returned, so it's not possible to manipulate the memory.
*/
rtems_status_code rtems_task_segmented_read_communication_memory_from_impl(Segmented_Task_Task* segmentedTask, uint8_t* buffer, size_t bufferSize,
                                                                        uint32_t startingAtByte);

/*
Read the whole communication memory of the given segmented task into the given buffer.
Boundary checks are performed, so therefor it is not possible to read out of the intended
memory.
A copy of the memory content is returned, so it's not possible to manipulate the memory.
*/
rtems_status_code rtems_task_segmented_read_communication_memory_impl(Segmented_Task_Task* segmentedTask, uint8_t* buffer, size_t bufferSize);                            

/*
Writes to the beginning of the allocated communication memory of the given segmented task.
Boundary checks are performed, so therefor it is not possible to write out of the intended
memory if the correct size in bytes of the content is given.
If the communication memory is fully zeroed beforehand depends on DO_ZERO_ON_WRITE.
*/
rtems_status_code rtems_task_segmented_write_communication_memory_impl(Segmented_Task_Task* segmentedTask, uint8_t* content, size_t sizeOfContent);

/*
Appends the given content after the last written content in the communication memory.
Boundary checks are performed, so therefor it is not possible to write out of the intended
memory if the correct size in bytes of the content is given.
*/
rtems_status_code rtems_task_segmented_append_communication_memory_impl(Segmented_Task_Task* segmentedTask, uint8_t* content, size_t sizeOfContent);

/*
"Empties" the communication memory of the segmented task. This means it's whole content is zeroed.
This also resets the nextByteToWrite information back to 0.
*/
rtems_status_code rtems_task_segmented_clear_communication_memory_impl(Segmented_Task_Task* segmentedTask);


#ifdef __cplusplus
}
#endif

#endif