#ifndef _RTEMS_RTEMS_SEGTASKSLFPIMPL_H
#define _RTEMS_RTEMS_SEGTASKSLFPIMPL_H

#include <stdio.h>

#include <rtems/rtems/segtaskimpl.h>
#include <rtems/rtems/segtaskslfpdata.h>
#include <rtems/rtems/attr.h>
#include <rtems/rtems/extended_rtems_status_codes.h>

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

TODO: Question / Nothing to fix right now.
Is there a disadvantage declaring it in the *.c file? It'll be still allocated on the
heap of the RTEMS OS right? Think this through, because u don't want to let this
variable get out of scope ever!
*/

/**
 * @brief RTEMS Segmented Task SLFP Implementation: Returns the index
 * of the task in the task pool.
 *
 * This routine returns the index of the segmented slfp task associated
 * with the given task id in the pool of segmented slfp tasks.
 * 
 * @param[in] task Pointer to the segmented slfp task.
 * @param[out] poolIndex Pointer to where the result should be stored.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_EXTENDED_NULL_POINTER if task or poolIndex is a null pointer.
 * @retval RTEMS_INVALID_ID if task is not present.
 */
rtems_extended_status_code getPoolIndexOfSLFPTask(Segmented_Task_SLFP_Task* task, uint32_t* poolIndex);

/**
 * @brief RTEMS Segmented Task SLFP Implementation: Reorder the task pool.
 *
 * This routine reorders the task pool and sets next index to the correct
 * index.
 * 
 * Attention:
 * This routine can only be called while interrupts are disabled, otherwise
 * it will result in an error.
 * 
 * @param[in] poolIndex The index of the task which was given back to the
 *                      pool which is the reason for the reordering.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_EXTENDED_INTERRUPTS_ENABLED if called while interrupts are
 * still enabled.
 * @retval RTEMS_INTERNAL_ERROR if an internal error occured.
 */
rtems_extended_status_code reorderSegmentedTasks(uint32_t poolIndex);

/**
 * @brief RTEMS Segmented Task SLFP Implementation: Return the task to the pool.
 *
 * This routine will return the task to the pool, after which it can be resused.
 * 
 * Attention:
 * This routine can only be called while interrupts are disabled, otherwise
 * it will result in an error.
 * 
 * @param[in] poolIndex The index of the task which was given back to the
 *                      pool which is the reason for the reordering.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_EXTENDED_INTERRUPTS_ENABLED if called while interrupts are
 * still enabled.
 * @retval RTEMS_EXTENDED_NULL_POINTER if task is a null pointer.
 * @retval RTEMS_EXTENDED_INTERNAL_ERROR if an internal error occured.
 * @retval RTEMS_INVALID_ID if the task pointed to by task is not present in
 *                          the task pool.
 */
rtems_extended_status_code returnSLFPTaskToPool(Segmented_Task_SLFP_Task* task);

/**
 * @brief RTEMS Segmented Task SLFP Implementation: Get task by id.
 *
 * This routine returns the segmented slfp task that is associated
 * with the given task id.
 * 
 * @param[in] id Id of the slfp task to retrieve.
 * @param[out] segmentedTaskToReturn Pointer that will contain
 *              the adress of the disired slfp task afterwards.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_EXTENDED_NULL_POINTER if semgentedTaskToReturn is a null Pointer.
 * For mapping on rtems_status_code see rtems_extended_status_code
 * details.
 * @retval RTEMS_INVALID_ID if no slfp task is associated with the
 * given id.
 */
rtems_extended_status_code getSegmented_Task_SLFP_Task(rtems_id id, Segmented_Task_SLFP_Task** segmentedTaskToReturn);

/**
 * @brief RTEMS Segmented SLFP Task Implementation: Get the priority
 * of the disired segment.
 *
 * This routine returns the priority of the segment that is associated
 * with the given index of the given segmented slfp task.
 * 
 * @param[in] task Pointer to the segmented slfp task.
 * @param[in] segmentIndex Index of the segment.
 * @param[out] priority Pointer to which the priority will be returned.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_EXTENDED_NULL_POINTER if task or priority is a null pointer.
 * For mapping on rtems_status_code see rtems_extended_status_code
 * details.
 * @retval RTEMS_EXTENDED_INVALID_INDEX  if segmentIndex is an invalid index.
 * For mapping on rtems_status_code see rtems_extended_status_code
 * details.
 */
rtems_extended_status_code getPriorityOfSegmentByIndex(Segmented_Task_SLFP_Task* task, uint32_t segmentIndex, rtems_task_priority* priority);

/**
 * @brief RTEMS Segmented SLFP Task Implementation: Empty the given segmented
 * task.
 * 
 * This routine sets all values of the given segmented task to their default
 * values. Attention: The TaskID is not changed at all, because the segmented
 * task could have a rtems task assigned with a valid id.
 * 
 * @param[in] givenSegmentedTask Pointer to the segmented task.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_EXTENDED_NULL_POINTER if segmentedTask is a null pointer.
 * For mapping on rtems_status_code see rtems_extended_status_code
 * details.
 * @retval RTEMS_INTERNAL_ERROR if an internal RTEMS inconsistency was detected.
 */
rtems_extended_status_code emptySegTaskSLFP(Segmented_Task_SLFP_Task* givenSegmentedTask);

/**
 * @brief RTEMS Segmented SLFP Task Implementation: Fill in the given data.
 *
 * This routines fills in the given data into the given segmented slfp task.
 * The values of the segmented slfp task are set to their default values
 * beforehand.
 * 
 * Attention: The id of the task is not changed at all, because the given
 * segmented task could have a rtems task assigned with a valid id.
 * 
 * Attention: taskStackSize, initialModes and taskAttributes are not validated
 * in this routine. Ensure, that the passed values are valid, otherwise the
 * filled data structure will be flawed.
 * 
 * @param[in] task Pointer to the segmented slfp task.
 * @param[in] taskName Name the given slfp task will receive.
 * @param[in] taskStackSize Size of the stack that is reserved for the
 *                          given slfp task.
 * @param[in] initialModes RTEMS modes that the given slfp task will
 *                          have at the beginning of it's execution.
 * @param[in] taskAttributes RTEMS attributes that the given slfp task will have.
 * @param[in] period The period in ticks with which the task should be
 *                   executed. Pass 0 if the task should not be executed
 *                   periodicly.
 * @param[in] numberOfSegments Number of segments the given slfp task is composed of.
 * @param[in] functionPointer Array that contains the functions of the segments
 *                          of the given slfp task. Its size must match the
 *                          numberOfSegments.
 * @param[in] priorities Array that contains the priorities of the segments of
 *                       the the given slfp task. It is important that this is
 *                       in the same order as functionPointer. Its size must
 *                       match the numberOfSegments.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull
 * @retval RTEMS_INVALID_PRIORITY if priorities contains an invalid rtems_task_priority.
 * @retval RTEMS_EXTENDED_NULL_POINTER if task is a null pointer or functionPointer
 *                                     contains a null pointer. For mapping on
 *                                     rtems_status_code see rtems_extended_status_code
 *                                     details.
 * @retval RTEMS_INTERNAL_ERROR if an internal RTEMS inconsistency was detected.
 * @retval RTEMS_INVALID_NAME if taskName is an invalid rtems_name.
 * @retval RTEMS_EXTENDED_TOO_MANY_SEGMENTS if numberOfSegments is greater than the
 *                                          maximum number configured. For mapping
 *                                          on rtems_status_code see rtems_extended_status_code
 *                                          details.
 * 
 * @par Notes:
 * taskStackSize, initialModes and taskAttributes are not validated, because it's not
 * possible from outside the system core. Therefore it is intended, that those values
 * are validated within rtems_task_create for example. If the corresponding routine
 * returns that part or all of those values are invalid, the invalid struct must be
 * removed.
 */
rtems_extended_status_code fillDataIntoSegTaskSLFP(Segmented_Task_SLFP_Task* task,
                rtems_name taskName, size_t taskStackSize,
                rtems_mode initialModes, rtems_attribute taskAttributes, rtems_interval period,
                uint32_t numberOfSegments, void (*functionPointer[]) (Segmented_Task_Arguments),
                rtems_task_priority priorities[]);

/**
 * @brief RTEMS Segmented SLFP Task Implementation: Returns the next free
 * segmented slfp task from the pool.
 *
 * This routine returns a pointer to the next free segmented
 * slfp task from the pool. This task is then "used". Therefore, next
 * time when this routine is called, the task after is returned.
 * 
 * @param[out] task Pointer to the segmented slfp task.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull
 * @retval RTEMS_TOO_MANY if all segmented slfp tasks of the pool
 *                        are in use already.
 * 
 * @par Notes:
 * The internal pointer on the next free task is increased when
 * calling this method.
 */
rtems_extended_status_code getNextFreeSLFPTaskFromPool(Segmented_Task_SLFP_Task** task);

/**
 * @brief RTEMS Segmented SLFP Task Implementation: Sets the priority
 * of the given segmented task.
 * 
 * This routine sets the priority of the given segmented slfp task pointed
 * to to the given priority.
 * 
 * @param[in] task Pointer to the segmented slfp task.
 * @param[in] priority Priority the given segmented slfp task
 *                     should be assigned to.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull
 * @retval RTEMS_EXTENDED_NULL_POINTER if the given pointer task is a null
 *                                     pointer.
 * @retval RTEMS_EXTENDED_INVALID_PRIORITY if the given priority is invalid.
 * @retval RTEMS_EXTENDED_INTERNAL_ERROR if an internal error occured.
 */
rtems_extended_status_code setSLFPTaskPriority(Segmented_Task_SLFP_Task* task, rtems_task_priority priority);

/**
 * @brief RTEMS Segmented Task SLFP Implementation: The runtime function.
 *
 * This routine implements the main runtime function that is executed as
 * a task by RTEMS. It will execute the segments of the segmented slfp
 * task and perform the self suspension between the tasks.
 * 
 * @param[in] arguments A set of user defined arguments for the task.
 * 
 * @par Notes
 * TODO:
 * - Implement loop for multiple execution of a segmented slfp task.
 */
void mainFunction(rtems_task_argument arguments);

/**
 * @brief RTEMS Segmented Task SLFP Implementation: The calling task
 * exists with a failure message.
 *
 * This routine is the fall back solution in case a task "fails". It
 * can then be exited with this routine instead of rtems_task_exit.
 * 
 * @param[in] taskId The ID of the calling task. Pass NULL if it's
 *                   unknown.
 * @param[in] errorMessage The error message to display. Pass NULL if
 *                         no message should be displayed.
 * 
 * @par Notes
 * TODO:
 * - The removeal from the task pool and everything connect to
 *   task deletion is missing beside RTEMS default operations.
 */
void rtems_exit_task_with_failure(rtems_id* taskId, char* errorMessage);

#ifdef __cplusplus
}
#endif

#endif