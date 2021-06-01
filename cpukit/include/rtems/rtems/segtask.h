#ifndef _RTEMS_RTEMS_SEGTASK_H
#define _RTEMS_RTEMS_SEGTASK_H

#include <rtems/rtems/segtaskslfp.h>

/**
 * Do this comments in this style
 * 
 * @brief RTEMS Set Task Priority
 *
 * This routine implements the rtems_task_set_priority directive. The
 * current priority of the task associated with ID is set to
 * new_priority. The former priority of that task is returned
 * in old_priority.
 *
 * @param[in] id is the thread to extract
 * @param[in] new_priority is the thread to extract
 * @param[in] old_priority is the thread to extract
 *
 * @retval RTEMS_SUCCESSFUL if successful or error code if unsuccessful and
 * and *old_priority filled in with the previous previous priority
 */

/*
Returns the size of the allocated memory for communcation between segments.

TODO:
So far each segmented task has the same sized communication memory. The result is therefor indepented of the given
segmented task.
*/
rtems_status_code rtems_task_segmented_get_communication_memory_size(size_t* size);

/*
Read the communication memory of the calling segmented task for the given number of bytes
starting at a specific byte (inclusive) into the given buffer.
Boundary checks are performed, so therefor it is not possible to read out of the intended
memory.
A copy of the memory content is returned, so it's not possible to manipulate the memory.
*/
rtems_status_code rtems_task_segmented_read_communication_memory_for_from(uint8_t* buffer, size_t bufferSize,
                                                                        size_t amount, uint32_t startingAtByte);

/*
Read the communication memory of the calling segmented task for the given number of bytes
starting at the beginning of the memory into the given buffer.
Boundary checks are performed, so therefor it is not possible to read out of the intended
memory.
A copy of the memory content is returned, so it's not possible to manipulate the memory.
*/
rtems_status_code rtems_task_segmented_read_communication_memory_for(uint8_t* buffer, size_t bufferSize,
                                                                        size_t amount);

/*
Read the whole communication memory of the calling segmented task starting at a specific
byte (inclusive) into the given buffer.
Boundary checks are performed, so therefor it is not possible to read out of the intended
memory.
A copy of the memory content is returned, so it's not possible to manipulate the memory.
*/
rtems_status_code rtems_task_segmented_read_communication_memory_from(uint8_t* buffer, size_t bufferSize,
                                                                        uint32_t startingAtByte);

/*
Read the whole communication memory of the calling segmented task into the given buffer.
Boundary checks are performed, so therefor it is not possible to read out of the intended
memory.
A copy of the memory content is returned, so it's not possible to manipulate the memory.
*/
rtems_status_code rtems_task_segmented_read_communication_memory(uint8_t* buffer, size_t bufferSize);                            

/*
Writes to the beginning of the allocated communication memory of the calling segmented task.
Boundary checks are performed, so therefor it is not possible to write out of the intended
memory if the correct size in bytes of the content is given.
If the communication memory is fully zeroed beforehand depends on DO_ZERO_ON_WRITE.
*/
rtems_status_code rtems_task_segmented_write_communication_memory(uint8_t* content, size_t sizeOfContent);

/*
Appends the given content after the last written content in the communication memory of the
calling segmented task.
Boundary checks are performed, so therefor it is not possible to write out of the intended
memory if the correct size in bytes of the content is given.
*/
rtems_status_code rtems_task_segmented_append_communication_memory(uint8_t* content, size_t sizeOfContent);

/*
"Empties" the communication memory of the calling segmented task. This means it's whole content is zeroed.
This also resets the nextByteToWrite information back to 0.
*/
rtems_status_code rtems_task_segmented_clear_communication_memory(void);

#endif