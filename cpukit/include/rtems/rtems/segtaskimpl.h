#ifndef _RTEMS_RTEMS_SEGTASKIMPL_H
#define _RTEMS_RTEMS_SEGTASKIMPL_H

#include <rtems/rtems/attr.h>
#include <rtems/rtems/segtaskdata.h>
#include <rtems/rtems/segtaskslfpimpl.h>
#include <rtems/rtems/extended_rtems_status_codes.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DO_ZERO_ON_WRITE 0

/**
 * @brief RTEMS Segmented Task Implementation: Checks wether the priority
 * is valid.
 *
 * This routine checks wether the given priority is a valid rtems_task_priority.
 * 
 * @param[in] priority Priority to validate.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_INVALID_PRIORITY if priority is not invalid.
 */
rtems_extended_status_code isPriorityValid(rtems_task_priority priority);

/**
 * @brief RTEMS Segmented Task Implementation: Execute the next segmented of
 * the given segmented task.
 *
 * This routine executes the next segment in line of the given segmented
 * task.
 * 
 * @param[in] task Pointer to the segmented task.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_EXTENDED_NULL_POINTER if task is a null pointer.
 * For mapping on rtems_status_code see rtems_extended_status_code
 * details.
 * @retval RTEMS_EXTENDED_FINAL_SEGMENT if the final segment of the task
 * was reached already.
 * For mapping on rtems_status_code see rtems_extended_status_code
 * details.
 */
rtems_extended_status_code executeNextSegment(Segmented_Task_Task* task);

/**
 * @brief RTEMS Segmented Task Implementation: Zeroes the given segmented
 * task.
 * 
 * This routine zeroes the given segmented task. Attention: The TaskID is
 * not changed at all, because the segmented task could have a rtems task
 * assigned with a valid id.
 * 
 * @param[in] task Pointer to the segmented task.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_EXTENDED_NULL_POINTER if task is a null pointer.
 * For mapping on rtems_status_code see rtems_extended_status_code
 * details.
 */
rtems_extended_status_code emptySegmentedTask(Segmented_Task_Task* task);

/**
 * @brief RTEMS Segmented Task Implementation: Get task by id.
 *
 * This routine returns the segmented task that is associated
 * with the given task id.
 * 
 * @param[in] id Id of the task to retrieve.
 * @param[in] segmentedTaskToReturn Pointer that will contain
 *              the adress of the disired task afterwards.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_INVALID_ID if no task is associated with the
 * given id.
 * @retval RTEMS_EXTENDED_NULL_POINTER if segmentedTaskToReturn is 
 * a null pointer.
 * For mapping on rtems_status_code see rtems_extended_status_code
 * details.
 */
rtems_extended_status_code getSegmented_Task_Task(rtems_id id, Segmented_Task_Task** segmentedTaskToReturn);

/**
 * @brief RTEMS Segmented Task Implementation: Fill in the given data.
 *
 * This routines fills in the given data into the given segmented task.
 * The values of the segmented task are set to their default values
 * beforehand.
 * 
 * Attention: The id of the task is not changed at all, because the given
 * segmented task could have a rtems task assigned with a valid id.
 * 
 * @param[in] task Pointer to the segmented slfp task.
 * @param[in] taskname Name the given task will receive.
 * @param[in] taskPriority Priority the given task will receive.
 * @param[in] taskStackSize Size of the stack that is reserved for the given task.
 * @param[in] initialModes RTEMS modes that the given task will have at the
 * beginning of it's execution.
 * @param[in] taskAttributes RTEMS attributes that the given task will have.
 * @param[in] numberOfSegments Number of segments the given task is composed of.
 * @param[in] functionPointer Array that contains the functions of the segments
 * of the given task. Its size must match the numberOfSegments.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_INVALID_NAME if taskName is an invalid rtems_name.
 * @retval RTEMS_INVALID_PRIORITY if taskPriority is an invalid rtems_task_priority.
 * @retval RTEMS_EXTENDED_NULL_POINTER if segmentedTaskToReturn is 
 * a null pointer or functionPointer contains a null pointer.
 * For mapping on rtems_status_code see rtems_extended_status_code
 * details.
 * @retval RTEMS_EXTENDED_TOO_MANY_SEGMENTS if numberOfSegments is 
 * greater than the maximum number configured.
 * For mapping on rtems_status_code see rtems_extended_status_code
 * details.
 */
rtems_extended_status_code fillDataIntoSegTask(Segmented_Task_Task* task,
                rtems_name taskName, rtems_task_priority taskPriority,
                size_t taskStackSize, rtems_mode initialModes, 
                rtems_attribute taskAttributes, uint32_t numberOfSegments,
                void (*functionPointer[]) (void));

/**
 * @brief RTEMS Segmented Task Implementation: Fill in the given general data.
 *
 * This routines fills in the given general data into the given segmented
 * task.
 * 
 * Attention: The id of the task is not changed at all, because the given
 * segmented task could have a rtems task assigned with a valid id.
 * 
 * @param[in] task Pointer to the segmented slfp task.
 * @param[in] taskname Name the given task will receive.
 * @param[in] taskPriority Priority the given task will receive.
 * @param[in] taskStackSize Size of the stack that is reserved for the given task.
 * @param[in] initialModes RTEMS modes that the given task will have at the
 * beginning of it's execution.
 * @param[in] taskAttributes RTEMS attributes that the given task will have.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_EXTENDED_NULL_POINTER if segmentedTaskToReturn is 
 * a null pointer.
 * For mapping on rtems_status_code see rtems_extended_status_code
 * details.
 * @retval RTEMS_INVALID_NAME if taskName is an invalid rtems_name.
 * @retval RTEMS_INVALID_PRIORITY if taskPriority is an invalid rtems_task_priority.
 */
rtems_extended_status_code fillGeneralDataIntoSegTask(Segmented_Task_Task* task,
                rtems_name taskName, rtems_task_priority taskPriority,
                size_t taskStackSize, rtems_mode initialModes, 
                rtems_attribute taskAttributes);

/**
 * @brief RTEMS Segmented Task Implementation: Fill in the given segment
 * specific data.
 *
 * This routines fills in the given segment specific data into the given
 * segmented task.
 * 
 * @param[in] task Pointer to the segmented slfp task.
 * @param[in] numberOfSegments Number of segments the given task is composed of.
 * @param[in] functionPointer Array that contains the functions of the segments
 * of the given task. Its size must match the numberOfSegments.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_EXTENDED_NULL_POINTER if task is 
 * a null pointer or functionPointer contains a null pointer.
 * For mapping on rtems_status_code see rtems_extended_status_code
 * details.
 * @retval RTEMS_EXTENDED_TOO_MANY_SEGMENTS if numberOfSegments is 
 * greater than the maximum number configured.
 * For mapping on rtems_status_code see rtems_extended_status_code
 * details.
 */
rtems_extended_status_code fillSegmentDataIntoSegTask(Segmented_Task_Task* task,
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