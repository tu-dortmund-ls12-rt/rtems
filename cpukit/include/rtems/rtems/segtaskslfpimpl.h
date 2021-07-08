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

TODO:
Is there a disadvantage declaring it in the *.c file? It'll be still allocated on the
heap of the RTEMS OS right? Think this through, because u don't want to let this
variable get out of scope ever!
*/

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
 * 
 * @par Notes
 * TODO:
 * Currently the given segmentedTask has no effect, because there is always only
 * one segmentedTask currently possible. So therefor it won't be used. This needs
 * to be changed when a list of Segmented_TASK_SLFP_Tasks is present.
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
 * @param[in] task Pointer to the segmented slfp task.
 * @param[in] taskName Name the given slfp task will receive.
 * @param[in] taskStackSize Size of the stack that is reserved for the
 *                          given slfp task.
 * @param[in] initialModes RTEMS modes that the given slfp task will
 *                          have at the beginning of it's execution.
 * @param[in] taskAttributes RTEMS attributes that the given slfp task will have.
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
 */
rtems_extended_status_code fillDataIntoSegTaskSLFP(Segmented_Task_SLFP_Task* task,
                rtems_name taskName, size_t taskStackSize,
                rtems_mode initialModes, rtems_attribute taskAttributes,
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

// TODO: Add description
// TODO: Handle cascading rtems_extended_status_codes in calling functions
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
 * - Renvame to avoid compiler warnings.
 * - Implement loop for multiple execution of a segmented slfp task.
 */
void main(rtems_task_argument arguments);

#ifdef __cplusplus
}
#endif

#endif