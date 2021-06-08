#ifndef _RTEMS_RTEMS_SEGTASKSLFP_H
#define _RTEMS_RTEMS_SEGTASKSLFP_H

#include <rtems/rtems/segtaskslfpdata.h>

#include <rtems.h>
#include <rtems/rtems/status.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief RTEMS Segmented SLFP Task: Create a segmented slfp task.
 *
 * This routine creates a segmented task with the given informations
 * that will be scheduled with Segmented Level Fixed Priority scheduling.
 * 
 * @param[in] taskname Name the task will receive.
 * @param[in] taskStackSize Size of the stack that is reserved for the task.
 * @param[in] initialModes RTEMS modes that the task will have at the
 * beginning of it's execution.
 * @param[in] taskAttributes RTEMS attributes that the task will have.
 * @param[in] numberOfSegments Number of segments the task is composed of.
 * @param[in] segmentFunctions Array that contains the functions of the segments
 * of the task. Its size must match the numberOfSegments.
 * @param[in] segmentPriorities Array that contains the priorities of the segments of
 * the the task. It is important that this is in the same order as functionPointer.
 * Its size must match the numberOfSegments.
 * @param[in] taskId Pointer to which the id of the task will be returned.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_INVALID_PRIORITY if segmentPriorities containss an invalid rtems_task_priority.
 * @retval RTEMS_INVALID_NAME if taskName is an invalid rtems_name.
 * @retval RTEMS_INVALID_ADDRESS if taskId is a null pointer or segmentFunctions contains
 * a null pointer.
 * @retval RTEMS_INTERNAL_ERROR if RTEMS inconsistency was detected.
 * @retval RTEMS_MP_NOT_CONFIGURED if multiprocessing is not configured.
 * @retval RTEMS_TOO_MANY if too many task are created.
 * @retval RTEMS_TOO_MANY if numberOfSegments is greater than the maximum number configured.
 * @retval RTEMS_UNSATISFIED if not enough memory for stack or floating point
 * context is present.
 * @retval RTEMS_UNSATISFIED if non-preemption mode is not supported on SMP system.
 * @retval RTEMS_UNSATISFIED if interrupt level mode is not supported on SMP system.
 * @retval RTEMS_TOO_MANY if there are too many global objects.
 */
rtems_status_code rtems_task_create_segmented_slfp(rtems_name taskName, size_t taskStackSize,
                rtems_mode initialModes, rtems_attribute taskAttributes,
                uint32_t numberOfSegments, void (*segmentFunctions[]) (void),
                rtems_task_priority segmentPriorities[], rtems_id* taskId);

/**
 * @brief RTEMS Segmented SLFP Task: Start the segmented slfp task by id.
 *
 * This routine starts the segmented task with the given id and will
 * schedule it by Segmented Level Fixed Priority scheduling
 * 
 * @param[in] taskId Id of the task that should be started.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_INVALID_ID if no slfp task is associated with the
 * given id.
 * @retval RTEMS_INCORRECT_STATE if task to start is not in the dormant state.
 * @retval RTEMS_ILLEGAL_ON_REMOTE_OBJECT if it's not possible to start a remote task.
 * 
 * @par Notes
 * TODO:
 * Currently can only one segmentedTask exists. So therefor the given id will have
 * effect.
 */
rtems_status_code rtems_task_start_segmented_slfp(rtems_id taskId);

/**
 * @brief RTEMS Segmented SLFP Task: Resume the segmented slfp task by id.
 *
 *  This routine resumes the previously suspended slfp task with the given id.
 * 
 * @param[in] taskId Id of the task that should be resumed.
 * 
 * @retval RTEMS_SUCCESSFUL if successfull.
 * @retval RTEMS_INVALID_ID if no slfp task is associated with the
 * given id.
 * @retval RTEMS_ILLEGAL_ON_REMOTE_OBJECT if the resume is not support
 * on remote tasks.
 * @retval RTEMS_INCORRECT_STATE if the task to resume is not suspended.
 */
rtems_status_code rtems_task_resume_segmented_slfp(rtems_id taskId);

#ifdef __cplusplus
}
#endif

#endif