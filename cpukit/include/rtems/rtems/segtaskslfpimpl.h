#ifndef _RTEMS_RTEMS_SEGTASKSLFPIMPL_H
#define _RTEMS_RTEMS_SEGTASKSLFPIMPL_H

#include <rtems/rtems/segtaskimpl.h>
#include <rtems/rtems/segtaskslfpdata.h>
#include <rtems/rtems/attr.h>
#include <rtems/rtems/extended_rtems_status_codes.h>
//#include <rtems/rtems/segtaskimpl.h>

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
 * @brief RTEMS Segmented SLFP Task Implementation: Get task by id.
 *
 * This routine returns the segmented slfp task that is associated
 * with the given task id.
 * 
 * @param[in] id Id of the slfp task to retrieve.
 * @param[in] segmentedTaskToReturn Pointer that will contain
 *              the adress of the disired task afterwards.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_INVALID_ID if no slfp task is associated with the
 * given id.
 * @retval RTEMS_EXTENDED_NULL_POINTER if semgentedTaskToReturn is a null Pointer.
 * For mapping on rtems_status_code see rtems_extended_status_code
 * details.
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
 * @param[in] priority Pointer to which the priority will be returned.
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
 * @brief RTEMS Segmented SLFP Task Implementation: Empties the segmented slfp task.
 * 
 * This routine sets all values of the given segmented slfp task to their default
 * values.
 * 
 * @param[in] givenSegmentedTask Pointer to the segmented slfp task.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_EXTENDED_NULL_POINTER if task is a null pointer.
 * For mapping on rtems_status_code see rtems_extended_status_code
 * details.
 * 
 * @par Notes
 * TODO:
 * Currently the given segmentedTask has no effect, because there is always only
 * segmentedTask currently possible. So therefor it won't be used. This needs
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
 * segmented slfp task could have a rtems task assigned with a valid id.
 * 
 * @param[in] task Pointer to the segmented slfp task.
 * @param[in] taskname Name the given task will receive.
 * @param[in] taskStackSize Size of the stack that is reserved for the given task.
 * @param[in] initialModes RTEMS modes that the given task will have at the
 * beginning of it's execution.
 * @param[in] taskAttributes RTEMS attributes that the given task will have.
 * @param[in] numberOfSegments Number of segments the given task is composed of.
 * @param[in] functionPointer Array that contains the functions of the segments
 * of the given task. Its size must match the numberOfSegments.
 * @param[in] priorities Array that contains the priorities of the segments of
 * the the given task. It is important that this is in the same order as functionPointer.
 * Its size must match the numberOfSegments.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_INVALID_PRIORITY if priorities containss an invalid rtems_task_priority.
 * @retval RTEMS_INVALID_NAME if taskName is an invalid rtems_name.
 * @retval RTEMS_EXTENDED_NULL_POINTER if task is 
 * a null pointer or functionPointer contains a null pointer.
 * For mapping on rtems_status_code see rtems_extended_status_code
 * details.
 * @retval RTEMS_EXTENDED_TOO_MANY_SEGMENTS if numberOfSegments is 
 * greater than the maximum number configured.
 * For mapping on rtems_status_code see rtems_extended_status_code
 * details.
 */
rtems_extended_status_code fillDataIntoSegTaskSLFP(Segmented_Task_SLFP_Task* task,
                rtems_name taskName, size_t taskStackSize,
                rtems_mode initialModes, rtems_attribute taskAttributes,
                uint32_t numberOfSegments, void (*functionPointer[]) (void),
                rtems_task_priority priorities[]);

/*
This is the main function which executes the segments,
performs the priority switching and the selfsuspension.

TODO:
Rename to avoid compiler warnings.
Implement loop for multiple execution of a segmented task.
*/
void main(rtems_task_argument arguments);

#ifdef __cplusplus
}
#endif

#endif