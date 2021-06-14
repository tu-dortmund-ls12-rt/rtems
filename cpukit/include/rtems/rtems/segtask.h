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

/**
 * @brief RTEMS Segmented Task: Get the size of the communication memory.
 *
 * This routine returns the size of the allocated memory of the calling
 * segmented task for the communication between its segments.
 * 
 * @param[out] size Size of the communication memory.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_INVALID_ADDRESS if size is a null pointer
 * @retval RTEMS_INTERNAL_ERROR if an internal RTEMS inconsistency was detected.
 */
rtems_status_code rtems_task_segmented_get_communication_memory_size(size_t* size); // CHECK

/**
 * @brief RTEMS Segmented Task: Read from the communication memory.
 *
 * This routine reads the content from the communication memory of the calling segmented
 * task for the given number of bytes starting at a specific byte (inclusive) into
 * the given buffer. Boundary checks are performed, so therefor it is not possible
 * to read outside of the intended memory. A copy of the memory content is returned,
 * so it's not possible to manipulate the memory accidently.
 * 
 * @param[out] buffer Pointer to the location where the content should be written to.
 * @param[in] bufferSize Size of the buffer in bytes.
 * @param[in] amount Amount of content to read in bytes.
 * @param[in] statingAtByte Byte where the reading should start.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_INTERNAL_ERROR if an internal RTEMS inconsistency was detected.
 * @retval RTEMS_INVALID_ADRESS if buffer is a null pointer.
 * @retval RTEMS_INVALID_SIZE if the buffer is to small to read the amount of content
 * defined by amount or a read outside of the communication memory should be performed.
 */
rtems_status_code rtems_task_segmented_read_communication_memory_for_from(uint8_t* buffer, size_t bufferSize,
                                                                        size_t amount, uint32_t startingAtByte); // CHECK

/**
 * @brief RTEMS Segmented Task: Read from the communication memory.
 *
 * This routine reads the content from the communication memory of the calling segmented
 * task for the given number of bytes starting at the beginning of the memory into
 * the given buffer. Boundary checks are performed, so therefor it is not possible
 * to read outside of the intended memory. A copy of the memory content is returned,
 * so it's not possible to manipulate the memory accidently.
 * 
 * @param[out] buffer Pointer to the location where the content should be written to.
 * @param[in] bufferSize Size of the buffer in bytes.
 * @param[in] amount Amount of content to read in bytes.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_INTERNAL_ERROR if an internal RTEMS inconsistency was detected.
 * @retval RTEMS_INVALID_ADRESS if buffer is a null pointer.
 * @retval RTEMS_INVALID_SIZE if the buffer is to small to read the amount of content
 * defined by amount or a read outside of the communication memory should be performed.
 */
rtems_status_code rtems_task_segmented_read_communication_memory_for(uint8_t* buffer, size_t bufferSize,
                                                                        size_t amount); // CHECK

/**
 * @brief RTEMS Segmented Task: Read from the communication memory.
 *
 * This routine reads the whole content from the communication memory of the calling
 * segmented task starting at a specific byte (inclusive) into the given buffer.
 * Boundary checks are performed, so therefor it is not possible to read outside
 * of the intended memory. A copy of the memory content is returned, so it's not
 * possible to manipulate the memory accidently.
 * 
 * @param[out] buffer Pointer to the location where the content should be written to.
 * @param[in] bufferSize Size of the buffer in bytes.
 * @param[in] statingAtByte Byte where the reading should start.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_INTERNAL_ERROR if an internal RTEMS inconsistency was detected.
 * @retval RTEMS_INVALID_ADRESS if buffer is a null pointer.
 * @retval RTEMS_INVALID_SIZE if the buffer is to small to read the amount of content
 * or a read outside of the communication memory should be performed.
 */
rtems_status_code rtems_task_segmented_read_communication_memory_from(uint8_t* buffer, size_t bufferSize,
                                                                        uint32_t startingAtByte); // CHECK

/**
 * @brief RTEMS Segmented Task: Read from the communication memory.
 *
 * This routine reads the whole content from the communication memory of the calling
 * segmented task into the given buffer. Boundary checks are performed, so therefor
 * it is not possible to read outside of the intended memory. A copy of the memory
 * content is returned, so it's not possible to manipulate the memory accidently.
 * 
 * @param[out] buffer Pointer to the location where the content should be written to.
 * @param[in] bufferSize Size of the buffer in bytes.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_INTERNAL_ERROR if an internal RTEMS inconsistency was detected.
 * @retval RTEMS_INVALID_ADRESS if buffer is a null pointer.
 * @retval RTEMS_INVALID_SIZE if the buffer is to small to read the amount of content
 * or a read outside of the communication memory should be performed.
 */
rtems_status_code rtems_task_segmented_read_communication_memory(uint8_t* buffer, size_t bufferSize); // CHECK                       

/**
 * @brief RTEMS Segmented Task: Writes content to communication memory.
 *
 * This routine writes to the beginning of the allocated communication memory of the
 * calling segmented task. Boundary checks are performed, so therefor it is not possible
 * to write outside of the intented memory if the correct size in bytes of the content
 * is given.
 * 
 * @param[in] content Pointer to the content.
 * @param[in] sizeOfContent Size of the content.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_INTERNAL_ERROR if an internal RTEMS inconsistency was detected.
 * @retval RTEMS_INVALID_ADRESS if content is a null pointer.
 * @retval RTEMS_INVALID_SIZE if sizeOfContent is zero or not enough
 * communication memory is present to write the whole content in the
 * communication memory.
 */
rtems_status_code rtems_task_segmented_write_communication_memory(uint8_t* content, size_t sizeOfContent); // CHECK

/**
 * @brief RTEMS Segmented Task: Append content to communication memory.
 *
 * This routine appends the given content after the last written content in the
 * communicationen memory of the calling segmented task. Boundary checks are performed,
 * so therefor it is not possible to write outside of the intented memory if the
 * correct size in bytes of the content is given.
 * 
 * @param[in] content Pointer to the content.
 * @param[in] sizeOfContent Size of the content.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_INTERNAL_ERROR if an internal RTEMS inconsistency was detected.
 * @retval RTEMS_INVALID_ADRESS if content is a null pointer.
 * @retval RTEMS_INVALID_SIZE if sizeOfContent is zero or not enough
 * communication memory is left to write the whole content in the
 * communication memory.
 */
rtems_status_code rtems_task_segmented_append_communication_memory(uint8_t* content, size_t sizeOfContent); // CHECK

/**
 * @brief RTEMS Segmented Task: Zeroes the communication memory
 * of the calling segmented task.
 * 
 * This routine zeroes the communication memory of the calling segmented task and
 * resets the nextByteToWrite information back to 0.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_INTERNAL_ERROR if an internal RTEMS inconsistency was detected.
 */
rtems_status_code rtems_task_segmented_clear_communication_memory(void); // CHECK

#endif