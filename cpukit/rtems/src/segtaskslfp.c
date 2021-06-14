#include <rtems/rtems/segtaskslfp.h>
#include <rtems/rtems/segtaskslfpimpl.h>

// ----- Global variable declaration / definition -----

Segmented_Task_SLFP_Task segmentedTask; // Currently only one segmented task is possible. Should be replaced with a dynamic data structure.

// ----- RTEMS API Implementation -----

rtems_status_code rtems_task_create_segmented_slfp(rtems_name taskName, size_t taskStackSize,
                rtems_mode initialModes, rtems_attribute taskAttributes, uint32_t numberOfSegments,
                void (*segmentFunctions[]) (void), rtems_task_priority segmentPriorities[], rtems_id* taskId) {
    // --- Argument validation ---
    // Arguments that need to be validated will be validated in fillDataIntoSegTaskSLFP and rtems_task_create

    // --- Implementation ---
    rtems_extended_status_code status;

    //TODO: Ditch the single task, and create a new one in a dynamic list
    status = emptySegTaskSLFP(&segmentedTask);
    if(!rtems_is_status_successful(status)) {
        return RTEMS_INTERNAL_ERROR; // RTEMS_INTERNAL_ERROR is returned, because the error is not dependet on the user input.
    }

    status = fillDataIntoSegTaskSLFP(&segmentedTask, taskName, taskStackSize, initialModes,
                taskAttributes, numberOfSegments, segmentFunctions, segmentPriorities);
    if(!rtems_is_status_successful(status)) {
        if(status == RTEMS_EXTENDED_NULL_POINTER && &segmentedTask == NULL) {
            return RTEMS_INTERNAL_ERROR; // RTEMS_INTERNAL_ERROR is returned, because the error is not dependet on the user input.
        } else {
            return status;
        }
    }

    status = rtems_task_create(taskName, segmentedTask.base.taskPriority, taskStackSize, initialModes, taskAttributes, taskId);
    if(!rtems_is_status_successful(status)) {
        if(status == RTEMS_INVALID_PRIORITY) {
            /*
            * RTEMS_INTERNAL_ERROR is returned, because the error is not dependet on the user input. A wrong
            * priority should be catched earlier.
            */
            return RTEMS_INTERNAL_ERROR;
        } else {
            return status;
        }
    }

    ((Segmented_Task_Task*)(&segmentedTask))->taskId = *taskId;

    return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_task_start_segmented_slfp(rtems_id taskId) {
    // --- Argument validation ---
    // Arguments that need to be validated will be validated in getSegmented_Task_SLFP_Task

    // --- Implementation ---
    rtems_extended_status_code status;
    /*
    Somehow redundant, but may be usefull in the future to check if taskId
    is associated with a segmented SLFP task.
    */
    Segmented_Task_SLFP_Task* receivedSegmentedTask;

    status = getSegmented_Task_SLFP_Task(taskId, &receivedSegmentedTask);
    if(!rtems_is_status_successful(status)) {
        return status;
    }

    Segmented_Task_Task* base = (Segmented_Task_Task*) receivedSegmentedTask;
    status = rtems_task_start(base->taskId, main, (rtems_task_argument) NULL);
    if(!rtems_is_status_successful(status)) {
        return status;
    }

    return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_task_resume_segmented_slfp(rtems_id taskId) {
    // --- Argument validation ---
    // Arguments that need to be validated will be validated in getSegmented_Task_SLFP_Task

    // --- Implementation ---
   rtems_extended_status_code status;
   rtems_task_priority nextPriority, wildCard;
   Segmented_Task_SLFP_Task* receivedTask;
   Segmented_Task_Task* receivedBase;
   
   status = getSegmented_Task_SLFP_Task(taskId, &receivedTask);
   if(!rtems_is_status_successful(status)) {
       return status;
   }

   status = rtems_task_is_suspended(taskId);
   if(status != RTEMS_ALREADY_SUSPENDED) {
       if(status != RTEMS_SUCCESSFUL) {
           return status;
       } else {
           return RTEMS_INCORRECT_STATE;
       }
   }

   status = getPriorityOfSegmentByIndex(receivedTask, receivedTask->base.currentSegment, &nextPriority);
   if(!rtems_is_status_successful(status)) {
       return status;
   }

   status = rtems_task_set_priority(taskId, nextPriority, &wildCard);
   if(!rtems_is_status_successful(status)) {
       return status;
   }

   return rtems_task_resume(taskId);
}

// ----- Hidden Implementation -----

rtems_extended_status_code getSegmented_Task_SLFP_Task(rtems_id id, Segmented_Task_SLFP_Task** segmentedTaskToReturn) {
    // --- Argument validation ---
    if(segmentedTaskToReturn == NULL) {
        return RTEMS_EXTENDED_NULL_POINTER;
    }

    // --- Implementation ---
    //TODO: Implement going through the list of all slfp tasks
    bool taskIsPresent = true; // TODO: Replace with real check
    if(!taskIsPresent) {
        return RTEMS_INVALID_ID;
    }

    *segmentedTaskToReturn = &segmentedTask;

    return RTEMS_SUCCESSFUL;
}

rtems_extended_status_code getPriorityOfSegmentByIndex(Segmented_Task_SLFP_Task* task, uint32_t segmentIndex, rtems_task_priority* priority) {
    // --- Argument validation ---
    if(task == NULL || priority == NULL) {
        return RTEMS_EXTENDED_NULL_POINTER;
    }

    if(segmentIndex + 1 > task->base.numberOfSegments) {
        return RTEMS_EXTENDED_INVALID_INDEX;
    }

    // --- Implementation ---
    *priority = task->priorities[segmentIndex];
    return RTEMS_SUCCESSFUL;
}

rtems_extended_status_code emptySegTaskSLFP(Segmented_Task_SLFP_Task* givenSegmentedTask) {
    // --- Argument validation ---
    /**
     * Arguments that need to be validated will be validated in emptySegmentedTask,
     */

    // --- Implementation ---
    rtems_extended_status_code status;

    status = emptySegmentedTask((Segmented_Task_Task*) givenSegmentedTask);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_EXTENDED_NULL_POINTER:
         *      User depented. Occurs when givenSegmentedTask is a null pointer.
         *      Must be forwarded.
         * RTEMS_INTERNAL_ERROR:
         *      User indepented. Must be forwarded.
         */
        return status;
    }

    for(uint32_t i = 0; i < CONFIGURE_MAXIMUM_SEGMENTS; i++) {
        givenSegmentedTask->priorities[i] = 0;
    }

    return RTEMS_SUCCESSFUL;
}

rtems_extended_status_code fillDataIntoSegTaskSLFP(Segmented_Task_SLFP_Task* task,
                rtems_name taskName, size_t taskStackSize,
                rtems_mode initialModes, rtems_attribute taskAttributes,
                uint32_t numberOfSegments, void (*functionPointer[]) (void),
                rtems_task_priority priorities[]) {
    // --- Argument validation ---
    /**
     * Other arguments that need to be validated will be validated in fillDataIntoSegTask.
     */
    for(uint32_t i = 0; i < numberOfSegments; i++) {
        if(!rtems_is_status_successful(isPriorityValid(priorities[i]))) {
            return RTEMS_INVALID_PRIORITY;
        }
    }

    // --- Implementation ---
    rtems_extended_status_code status;

    status = fillDataIntoSegTask((Segmented_Task_Task*) task, taskName, priorities[0], taskStackSize, initialModes, taskAttributes, numberOfSegments, functionPointer);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_EXTENDED_NULL_POINTER:
         *      User depented. Occurs when task is a null pointer or functionPointer contains
         *      a null pointer. Must be forwarded.
         * RTEMS_INTERNAL_ERROR:
         *      User indepented. Must be forwarded.
         * RTEMS_INVALID_NAME:
         *      User dependent. Must be forwarded.
         * RTEMS_INVALID_PRIORITY:
         *      User dependent, but was validated beforehand. Therefor in case of this
         *      error an internal error must be returned.
         * RTEMS_EXTENDED_TOO_MANY_SEGMENTS:
         *      User dependent. Must be forwarded.
         */
        if(status == RTEMS_INVALID_PRIORITY) {
            return RTEMS_INTERNAL_ERROR;
        } else {
            return status;
        }
    }

    for(uint32_t i = 0; i < numberOfSegments; i++) {
        (task->priorities)[i] = priorities[i];
    }

    return RTEMS_SUCCESSFUL;
}

void main(rtems_task_argument arguments) {
    // --- Argument validation ---
    
    // --- Implementation
    Segmented_Task_SLFP_Task* segmentedTask;
    rtems_extended_status_code status;
    
    status = getSegmented_Task_SLFP_Task(RTEMS_SELF, &segmentedTask);
    if(!rtems_is_status_successful(status)) {
        /**
         * When an error happens the RTEMS task must be exited with a failure.
         * Currently a fitting method can't be found. Therefor only an information
         * is displayed and the task is exited in a regular way.
         * 
         * TODO:
         * Find a real way to exit the task with a failure.
         */
       printf("\n\n--- ERROR ---!\n");
       printf("Task %u exited with failure. Currently there is no way to exit in an irregular way.\n", RTEMS_SELF);
       printf("-------");
       printf("\n\n");
       rtems_task_exit();
    }

    Segmented_Task_Task* base = (Segmented_Task_Task*) segmentedTask;
    for(uint32_t i = 0; i < base->numberOfSegments; i++) {
        executeNextSegment(base);
        
        // No suspension is needed after the last segment.
        if(i != base->numberOfSegments - 1) {
            rtems_task_suspend(RTEMS_SELF);
        }
    }
    rtems_task_exit();
}