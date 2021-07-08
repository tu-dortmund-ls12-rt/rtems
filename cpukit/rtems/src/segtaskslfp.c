#include <rtems/rtems/segtaskslfp.h>
#include <rtems/rtems/segtaskslfpimpl.h>

// ----- Global variable declaration / definition -----

/*
TODO:
WORKAROUND!

Same problem as described for CONFIGURE_MAXIMUM_SEGMENTS described in
segtaskdata.h does also apply for CONFIGURE_MAXIMUM_TASKS.
*/
#define CONFIGURE_MAXIMUM_TASKS 10

/**
 * TODO:
 * Global variables needs to be protected against
 * raceconditions.
 */
Segmented_Task_SLFP_Task segmentedTasks[CONFIGURE_MAXIMUM_TASKS];
uint32_t next = 0; // Pointer to the next unused segmented slfp task in the pool segmentedTasks.

// ----- RTEMS API Implementation -----

rtems_status_code rtems_task_create_segmented_slfp(rtems_name taskName, size_t taskStackSize,
                rtems_mode initialModes, rtems_attribute taskAttributes, uint32_t numberOfSegments,
                void (*segmentFunctions[]) (Segmented_Task_Arguments), rtems_task_priority segmentPriorities[], rtems_id* taskId) {
    // --- Argument validation ---
    /**
     * Arguments that need to be validated will be validated in fillDataIntoSegTaskSLFP and rtems_task_create.
     */

    // --- Implementation ---
    rtems_extended_status_code status;
    Segmented_Task_SLFP_Task* segmentedTask;

    status = getNextFreeSLFPTaskFromPool(&segmentedTask);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_TOO_MANY:
         *      User dependent. Needs to be forwarded.
         */
        return status;
    } 

    status = emptySegTaskSLFP(segmentedTask); // TODO: Maybe should be done if a task is given back to the pool.
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_EXTENDED_NULL_POINTER:
         *      Not user depented. Therefor it is an internal error.
         * RTEMS_INTERNAL_ERROR:
         *      Needs to be forwarded.
         */
        return RTEMS_INTERNAL_ERROR;
    }

    status = fillDataIntoSegTaskSLFP(segmentedTask, taskName, taskStackSize, initialModes,
                taskAttributes, numberOfSegments, segmentFunctions, segmentPriorities);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_INVALID_PRIORITY:
         *      User dependent. Needs to be forwarded.
         * RTEMS_EXTENDED_NULL_POINTER:
         *      Can be user depenedent or not, depending on where the null pointer occurs.
         *      Has to be split up into a forwarded error and an internal error.
         * RTEMS_INTERNAL_ERROR:
         *      Needs to be forwarded.
         * RTEMS_INVALID_NAME:
         *      User dependent. Needs to be forwarded.
         * RTEMS_EXTENDED_TOO_MANY_SEGMENTS:
         *      User dependent. Needs to be forwarded.
         */
        if(status == RTEMS_EXTENDED_NULL_POINTER && &segmentedTask == NULL) {
            return RTEMS_INTERNAL_ERROR;
        } else {
            return status;
        }
    }

    status = rtems_task_create(taskName, (segmentedTask->base).taskPriority, taskStackSize, initialModes, taskAttributes, taskId);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_INVALID_ADDRESS:
         *       User dependent. Needs to be forwarded.
         * RTEMS_INVALID_NAME:
         *       User dependent but validated beforehand. Therefor an internal error
         *       must be returned.
         * RTEMS_INVALID_PRIORITY:
         *       User dependent but validated beforehand. Therefor an internal error
         *       must be returned.
         * RTEMS_MP_NOT_CONFIGURED:
         *       User dependent. Needs to be forwarded.
         * RTEMS_TOO_MANY:
         *       User dependent. Needs to be forwarded.
         * RTEMS_UNSATISFIED:
         *       User dependent. Needs to be forwarded.
         */
        if(status == RTEMS_INVALID_NAME || status == RTEMS_INVALID_PRIORITY) {
            return RTEMS_INTERNAL_ERROR;
        } else {
            return status;
        }
    }

    ((Segmented_Task_Task*)(segmentedTask))->taskId = *taskId;

    return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_task_start_segmented_slfp(rtems_id taskId, rtems_task_argument taskArguments) {
    // --- Argument validation ---
    /**
     * Arguments that need to be validated will be validated in getSegmented_Task_SLFP_Task.
     */

    // --- Implementation ---
    rtems_extended_status_code status;
    /**
     * Somehow redundant, but may be usefull in the future to check if taskId
     * is associated with a segmented SLFP task.
     */
    Segmented_Task_SLFP_Task* receivedSegmentedTask = NULL;
    Segmented_Task_Task* base = NULL;

    status = getSegmented_Task_SLFP_Task(taskId, &receivedSegmentedTask);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_EXTENDED_NULL_POINTER:
         *      Not user depented. Therefor it is an internal error.
         * RTEMS_INVALID_ID:
         *      User dependent. Needs to be forwarded.
         */
        if(status == RTEMS_EXTENDED_NULL_POINTER) {
            return RTEMS_INTERNAL_ERROR;
        } else {
            return status;
        }
    }

    base = (Segmented_Task_Task*) receivedSegmentedTask;
    status = rtems_task_start(base->taskId, main, taskArguments);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_INVALID_ADDRESS:
         *      Not user depented. Therefor it is an internal error.
         * RTEMS_INVALID_ID:
         *      User dependent but validated beforhand. Therefor it is an internal error.
         * RTEMS_INCORRECT_STATE:
         *      User dependent. Needs to be forwarded.
         * RTEMS_ILLEGAL_ON_REMOTE_OBJECT:
         *      User dependent. Needs to be forwarded.
         */
        if(status == RTEMS_INVALID_ADDRESS || status == RTEMS_INVALID_ID) {
            return RTEMS_INTERNAL_ERROR;
        } else {
            return status;
        }
    }

    return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_task_resume_segmented_slfp(rtems_id taskId) {
    // --- Argument validation ---
    /**
     * Arguments that need to be validated will be validated in getSegmented_Task_SLFP_Task.
     */

    // --- Implementation ---
   rtems_extended_status_code status;
   rtems_task_priority nextPriority;
   Segmented_Task_SLFP_Task* receivedTask;
   Segmented_Task_Task* receivedBase;
   
   status = getSegmented_Task_SLFP_Task(taskId, &receivedTask);
   if(!rtems_is_status_successful(status)) {
       /**
        * Possible errors:
        * RTEMS_EXTENDED_NULL_POINTER:
        *      Not user depented. Therefor it is an internal error.
        * RTEMS_INVALID_ID:
        *      User dependent. Needs to be forwarded.
        */
       if(status == RTEMS_EXTENDED_NULL_POINTER) {
           return RTEMS_INTERNAL_ERROR;
       } else {
           return status;
       }
   }

   status = rtems_task_is_suspended(taskId);
   if(status != RTEMS_ALREADY_SUSPENDED) {
       if(status != RTEMS_SUCCESSFUL) {
           return status;
       } else {
           return RTEMS_INCORRECT_STATE;
       }
   }

   status = getPriorityOfSegmentByIndex(receivedTask, receivedTask->base.currentSegment + 1, &nextPriority);
   if(!rtems_is_status_successful(status)) {
       /**
        * Possible errors:
        * RTEMS_EXTENDED_NULL_POINTER:
        *      User dependent and validated beforhand or user independent. In both cases it
        *      is an internal error.
        * RTEMS_EXTENDED_INVALID_INDEX:
        *      User independent. Therefor it is an internal error.
        */
       return RTEMS_INTERNAL_ERROR;
   }

   status = setSLFPTaskPriority(receivedTask, nextPriority);
   if(!rtems_is_status_successful(status)) {
       /**
        * TODO: Change errors to fit setSLFPTaskPriority errors.
        * 
        * Possible errors:
        * RTEMS_INVALID_ID:
        *      User dependent but validated beforhand. Therefor it is an internal error.
        * RTEMS_INVALID_ADDRESS:
        *      User independent. Therefor it is an internal error.
        * RTEMS_INVALID_PRIORITY:
        *      User independent. Therefor it is an internal error.
        */
       return status;
   }

   status = rtems_task_resume(taskId);
   if(!rtems_is_status_successful(status)) {
       /**
        * Possible errors:
        * RTEMS_INVALID_ID:
        *       User dependent but validated beforhand. Therefor it is an internal error.
        * RTEMS_INCORRECT_STATE:
        *       User call dependented but validated beforehand. Therefor it is an internal
        *       error.
        */
       return RTEMS_INTERNAL_ERROR;
   }

   return RTEMS_SUCCESSFUL;
}

// ----- Hidden Implementation -----

rtems_extended_status_code getSegmented_Task_SLFP_Task(rtems_id id, Segmented_Task_SLFP_Task** segmentedTaskToReturn) {
    // --- Argument validation ---
    if(segmentedTaskToReturn == NULL) {
        return RTEMS_EXTENDED_NULL_POINTER;
    }

    if(id == 0) {
        return RTEMS_INVALID_ID;
    }

    // --- Implementation ---
    Segmented_Task_SLFP_Task* currentTask = NULL;
    Segmented_Task_Task* currentBase = NULL;
    bool taskIsPresent = false;

    for(uint32_t i = 0; i < CONFIGURE_MAXIMUM_TASKS; i++) {
        currentTask = &(segmentedTasks[i]);
        currentBase = (Segmented_Task_Task*) currentTask;
        if(currentBase->taskId == id) {
            taskIsPresent = true;
            break;
        }
    }

    if(!taskIsPresent) {
        return RTEMS_INVALID_ID;
    }

    *segmentedTaskToReturn = currentTask;

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
                uint32_t numberOfSegments, void (*functionPointer[]) (Segmented_Task_Arguments),
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

rtems_extended_status_code getNextFreeSLFPTaskFromPool(Segmented_Task_SLFP_Task** task) {
    // --- Argument validation ---
    if(next++ == CONFIGURE_MAXIMUM_TASKS) {
        return RTEMS_TOO_MANY;
    }

    // --- Implementation ---
    *task = &(segmentedTasks[next]);

    return RTEMS_SUCCESSFUL;
}

rtems_extended_status_code setSLFPTaskPriority(Segmented_Task_SLFP_Task* task, rtems_task_priority priority) {
    // --- Argument validation ---
    /**
     * Other arguments that need to be validated will be validated in rtems_task_set_priority,
     */

    if(task == NULL) {
        return RTEMS_EXTENDED_NULL_POINTER;
    }

    // --- Implementation ---
    rtems_task_priority wildCard;

    task->base.taskPriority = priority;
    rtems_task_set_priority(task->base.taskId, priority, &wildCard);

    return RTEMS_SUCCESSFUL;
}

void main(rtems_task_argument arguments) {
    // --- Argument validation ---
    
    // --- Implementation
    Segmented_Task_SLFP_Task* segmentedTask;
    rtems_extended_status_code status;
    rtems_id ownId;
    
    status = rtems_task_ident(RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &ownId);
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
       printf("An unkown task exited with failure. Currently there is no way to exit in an irregular way.\n");
       printf("-------");
       printf("\n\n");
       rtems_task_exit();
    }

    status = getSegmented_Task_SLFP_Task(ownId, &segmentedTask);
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
       printf("Task %u exited with failure. Currently there is no way to exit in an irregular way.\n", ownId);
       printf("-------");
       printf("\n\n");
       rtems_task_exit();
    }

    Segmented_Task_Task* base = (Segmented_Task_Task*) segmentedTask;
    for(uint32_t i = 0; i < base->numberOfSegments; i++) {
        status = executeNextSegment(base, arguments);
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
            printf("Task %u exited with failure. Currently there is no way to exit in an irregular way.\n", ownId);
            printf("-------");
            printf("\n\n");
            rtems_task_exit();
        }
        
        // No suspension is needed after the last segment.
        if(i != base->numberOfSegments - 1) {
            rtems_task_suspend(RTEMS_SELF); // Using RTEMS_SELF is fine, because it is used as a flag.
        }
    }
    
    rtems_task_exit();
}