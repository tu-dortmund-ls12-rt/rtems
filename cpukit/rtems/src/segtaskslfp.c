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
        if(status == RTEMS_EXTENDED_NULL_POINTER && segmentedTask == NULL) {
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
        if((rtems_status_code) status == RTEMS_INVALID_NAME || (rtems_status_code) status == RTEMS_INVALID_PRIORITY) {
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
    /*
     * TODO: Implementaiton: Potential bug.
     * Is the memory, pointed to by taskArguments, copied by rtems_task_start
     * to a memory region that will stay in scope? If not, then if the task
     * arguments are created locally on the stack during the initial Task
     * instead of declared as a global variable, the arguments will be out
     * of scope as soon, as the initial Task will exit, which will happen
     * before the execution of any other Task.
     */
    status = rtems_task_start(base->taskId, mainFunction, taskArguments);
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
        if((rtems_status_code) status == RTEMS_INVALID_ADDRESS || (rtems_status_code) status == RTEMS_INVALID_ID) {
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
   if((rtems_status_code) status != RTEMS_ALREADY_SUSPENDED) {
       if((rtems_status_code) status != RTEMS_SUCCESSFUL) {
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
        * Possible errors:
        * RTEMS_EXTENDED_NULL_POINTER:
        *      User dependent but validated beforehand. Therefore it is an internal error.
        * RTEMS_EXTENDED_INVALID_PRIORITY:
        *      User dependent, but as long as the initial validity check of the priority
        *      was performed while creating the segmented slfp task, this should not be
        *      possible. Therefore it is an internal error.
        * RTEMS_EXTENDED_INTERAL_ERROR:
        *      User independent. Muste be forwarded.
        */
       return RTEMS_INTERNAL_ERROR;
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

void rtems_task_exit_segmented_slfp(void) {
    // --- Implementation ---
    rtems_extended_status_code status;
    rtems_id ownId;
    Segmented_Task_SLFP_Task* task = NULL;

    /*
     * Cleaning the task, giving it back and reordering the pool should be atomic
     * to ensure the integrity of the pool.
     * 
     * For the rtems_task_exit() call interrupts need to be enabled, otherwise it
     * could result in a fatal error. Therefore it cannot be part of the atomic
     * section. But an interrupt between the atomic section and the exit call
     * shouldn't be problematic, because nothing can really happen.
     * 
     * TODO: Implementation / Race conditions
     * Check your assumtion when you focus on the race conditions.
     */
    // ----- Atomic section start -----
    status = rtems_task_ident(RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &ownId);
    if(!rtems_is_status_successful(status)) {
        /*
         * In case of an error, no error handling is possible. One cannot aboart
         * the exit operation. Therefor print, that an error happend and ensure
         * the task is actually exited. The pool will be flawed afterwards.
         */
        // ----- Atomic section end -----
        char* errorMessage = "An internal error occured during the exit of the task. The pool will be flawed from here on.";
        rtems_exit_task_with_failure(NULL, errorMessage);
        // No return needed, task will be exited and not continue.
    }

    status = getSegmented_Task_SLFP_Task(ownId, &task);
    if(!rtems_is_status_successful(status)) {
        /*
         * In case of an error, no error handling is possible. One cannot aboart
         * the exit operation. Therefor print, that an error happend and ensure
         * the task is actually exited. The pool will be flawed afterwards.
         */
        // ----- Atomic section end -----
        char* errorMessage = "An internal error occured during the exit of the task. The pool will be flawed from here on.";
        rtems_exit_task_with_failure(NULL, errorMessage);
        // No return needed, task will be exited and not continue.
    }

    status = returnSLFPTaskToPool(task);
    if(!rtems_is_status_successful(status)) {
        /*
         * In case of an error, no error handling is possible. One cannot aboart
         * the exit operation. Therefor print, that an error happend and ensure
         * the task is actually exited. The pool will be flawed afterwards.
         */
        // ----- Atomic section end -----
        char* errorMessage = "An internal error occured during the exit of the task. The pool will be flawed from here on.";
        rtems_exit_task_with_failure(NULL, errorMessage);
        // No return needed, task will be exited and not continue.
    }
    // ----- Atomic section end -----

    /*
     * Has to be the last call, otherwise the rest is
     * not executed.
     */
    rtems_task_exit();
}

// ----- Hidden Implementation -----
rtems_extended_status_code returnSLFPTaskToPool(Segmented_Task_SLFP_Task* task) {
    /*
     * TODO: Implementation / Potential Bug
     * The reording can cause wrong task handling if interrupt happens at bad times.
     * Example Case:
     * Task 1 -> Currently uses pointer to task from pool at adress 0x6
     * Task 2 -> Interrupts Task 1 and exits, resulting in a reordering of the pool.
     *           The task Task 1 pointed to is now at 0x5 but Task 1 still points to
     *           0x6, resulting in working with the wrong task.
     */

    // --- Argument validation ---
    /*
     * TODO: Implementation / Race Conditions
     * Can only be called while interrupts are disabled. Proposed
     * a version in the following comment.
     */
    /* if(areInterruptsEnabled()) {
     *    return RTEMS_EXTENDED_INTERRUPTS_ENABLED;
     * }
     */

    if(task == NULL) {
        return RTEMS_EXTENDED_NULL_POINTER;
    }

    // --- Implementation ---
    rtems_extended_status_code status;
    uint32_t poolIndex;

    status = getPoolIndexOfSLFPTask(task, &poolIndex);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_EXTENDED_NULL_POINTER:
         *      User dependent, but validated beforehand.
         *      Therefore it is an internal error.
         * RTEMS_INVALID_ID:
         *      The user has looked for a task that is not
         *      present. Therefore it needs to be forwarded.
         */
        if(status == RTEMS_EXTENDED_NULL_POINTER) {
            return RTEMS_INTERNAL_ERROR;
        } else {
            return status;
        }
    }

    status = reorderSegmentedTasks(poolIndex);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_EXTENDED_INTERRUPTS_ENABLED:
         *      Should not be possible, because this rountine
         *      could also only be executed if interrupts are
         *      disabled. So therefore it is an internal error.
         * RTEMS_EXTENDED_INTERNAL_ERROR:
         *      Needs to be forwarded.
         */
        return RTEMS_INTERNAL_ERROR;
    }

    return RTEMS_SUCCESSFUL;
}

rtems_extended_status_code getPoolIndexOfSLFPTask(Segmented_Task_SLFP_Task* task, uint32_t* poolIndex) {
    // --- Argument validation ---
    if(task == NULL || poolIndex == NULL) {
        return RTEMS_EXTENDED_NULL_POINTER;
    }

    // --- Implementation ---
    Segmented_Task_Task* base = (Segmented_Task_Task*) task;
    Segmented_Task_SLFP_Task* currentTask = NULL;
    Segmented_Task_Task* currentBase = NULL;
    bool taskIsPresent = false;

    for(uint32_t i = 0; i < CONFIGURE_MAXIMUM_TASKS; i++) {
        currentTask = &(segmentedTasks[i]);
        currentBase = (Segmented_Task_Task*) currentTask;
        if(currentBase->taskId == base->taskId) {
            taskIsPresent = true;
            *poolIndex = i;
            break;
        }
    }

    if(!taskIsPresent) {
        return RTEMS_INVALID_ID;
    } else {
        return RTEMS_SUCCESSFUL;
    }
}

rtems_extended_status_code reorderSegmentedTasks(uint32_t poolIndex) {
    // --- Argument Validation ---
    /*
     * TODO: Implementation / Race Conditions
     * Check if interrupts are still enabled. If the are
     * return the following which is commented right now.
     */
    /* if(interruptsAreStillEnabled()) {
     *     return RTEMS_EXTENDED_INTERRUPTS_ENABLED;
     * }
     */

    if(poolIndex < 0 || poolIndex >= CONFIGURE_MAXIMUM_TASKS) {
        return RTEMS_EXTENDED_INVALID_INDEX;
    }

    // --- Implementation ---
    rtems_extended_status_code status;
    uint32_t toReplace = poolIndex;
    uint32_t toMove = toReplace + 1;
    Segmented_Task_SLFP_Task* task;

    while(toMove != next) {
        segmentedTasks[toReplace++] = segmentedTasks[toMove++];
    }

    next = toReplace;
    task = &(segmentedTasks[next]);

    status = emptySegTaskSLFP(task);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_EXTENDED_NULL_POINTER:
         *      Not user dependent. Therefore it is an internal error.
         * RTEMS_INTERNAL_ERROR:
         *      Needs to be forwarded.
         */
        return RTEMS_INTERNAL_ERROR;
    }

    return RTEMS_SUCCESSFUL;
}

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
        if(!isPriorityValid(priorities[i])) {
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
        if((rtems_status_code) status == RTEMS_INVALID_PRIORITY) {
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
    if(next == CONFIGURE_MAXIMUM_TASKS) {
        return RTEMS_TOO_MANY;
    }

    // --- Implementation ---
    *task = &(segmentedTasks[next++]);

    return RTEMS_SUCCESSFUL;
}

rtems_extended_status_code setSLFPTaskPriority(Segmented_Task_SLFP_Task* task, rtems_task_priority priority) {
    // --- Argument validation ---
    if(task == NULL) {
        return RTEMS_EXTENDED_NULL_POINTER;
    }


    if(!isPriorityValid(priority)) {
        return RTEMS_INVALID_PRIORITY;
    }

    // --- Implementation ---
    rtems_task_priority wildCard;
    rtems_extended_status_code status;
    rtems_task_priority originalPriority;

    /*
     * Segmented slfp task structure properties must be changed first,
     * because the rtems_task_set_priority function call could result in
     * a task switch due to the scheduler.
     * Therefore, the validity check of the priority can not be done by
     * the rtems_task-set_priority function call.
     * 
     * TODO:
     * Thread safety. Switching the priority of the task by calling
     * rtems_tassks_set_priority and the update of the segmented slfp
     * task data structure must be atomic. Because otherwise it could
     * happen, that the data structure is updated first, then an
     * interrupt happens, switches out the task of execution, and the
     * task is then handled by the priority in the data structure by
     * the code, even though in reality it still has a different
     * priority. Other than the interrupt also just a task switch could
     * happen in between (which, in the end, is once again caused by an
     * interrupt). So basicly: Disable interrupts for a short duration.
     * This should also disable possible task switches, because the
     * scheduler can only react to interrupts or direct function calls.
     * Just ensure, the two operation are atomic together.
     */

    // --- ATOMICITY START
    originalPriority = priority;
    task->base.taskPriority = priority;
    status = rtems_task_set_priority(task->base.taskId, priority, &wildCard);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_INVALID_ID:
         *      Not user dependent. If task is a valid task pointer, then the underlying
         *      structure should never be able to have an invalid id.
         * RTEMS_INVALID_ADDRESS:
         *      Not user dependent.
         * RTEMS_INVALID_PRIORITY:
         *      User dependent. Must be forwarded.
         * RTEMS_INVALID_PRIORITY:
         *      User dependent, but validated before hand.
         */
        if((rtems_status_code) status == RTEMS_INVALID_PRIORITY) {
            /*
             * In case the operation fails due to an internal error
             * all changes done so far must be reverted.
             */
            task->base.taskPriority = originalPriority;
        }

        return RTEMS_INTERNAL_ERROR;
    }
    // --- ATOMICITY END (or early before a return statement)

    return RTEMS_SUCCESSFUL;
}

void mainFunction(rtems_task_argument arguments) {
    // --- Argument validation ---
    
    // --- Implementation
    Segmented_Task_SLFP_Task* segmentedTask;
    rtems_extended_status_code status;
    rtems_id ownId;
    
    status = rtems_task_ident(RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &ownId);
    if(!rtems_is_status_successful(status)) {
        char* errMsg = "ID of the calling task couldn't be retrieved. Internal error.";
        rtems_exit_task_with_failure(NULL, errMsg);
    }

    status = getSegmented_Task_SLFP_Task(ownId, &segmentedTask);
    if(!rtems_is_status_successful(status)) {
        char* errMsg = "Associated task couldn't be retrieved. Internal error.";
        rtems_exit_task_with_failure(&ownId, errMsg);
    }

    Segmented_Task_Task* base = (Segmented_Task_Task*) segmentedTask;
    for(uint32_t i = 0; i < base->numberOfSegments; i++) {
        status = executeNextSegment(base, arguments);
        if(!rtems_is_status_successful(status)) {
            char* errMsg = "The next segment of the calling task couldn't be executed. Internal error.";
            rtems_exit_task_with_failure(&ownId, errMsg);
        }
        
        // No suspension is needed after the last segment.
        if(i != base->numberOfSegments - 1) {
            rtems_task_suspend(RTEMS_SELF); // Using RTEMS_SELF is fine, because it is used as a flag.
        }
    }
    
    rtems_task_exit_segmented_slfp();
}

void rtems_exit_task_with_failure(rtems_id* taskId, char* errorMessage) {
    // --- Argument validation ---
    
    // --- Implementation
    /*
     * TODO: Implementation
     * Printf is used here for an easy access and quick solution. This
     * could possibly result in an unwanted behavior (unknown to me).
     * Maybe switch this out in the future.
     */
    printf("\n\n--- ERROR ---!\n");
    if(taskId == NULL) {
        printf("Task with an unknown ID exited with failure.\n");
    } else {
        printf("Task %u exited with failure.\n", *taskId);
    }
    if(errorMessage != NULL) {
        printf("Error message:    %s\n", errorMessage);
    }
    printf("--------------");
    printf("\n\n");

    rtems_task_exit();
}