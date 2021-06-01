#include <rtems/rtems/segtaskslfp.h>
#include <rtems/rtems/segtaskslfpimpl.h>

// ----- Global variable declaration / definition -----

Segmented_Task_SLFP_Task segmentedTask; // Currently only one segmented task is possible. Should be replaced with a dynamic data structure.

// ----- RTEMS API Implementation -----

rtems_status_code rtems_task_create_segmented_slfp(rtems_name taskName, size_t taskStackSize,
                rtems_mode initialModes, rtems_attribute taskAttributes, uint32_t numberOfSegments,
                void (*segmentFunctions[]) (void), rtems_task_priority segmentPriorities[], rtems_id* taskId) {
    rtems_extended_status_code status;
    
    //TODO: Ditch the single task, and create a new one in a dynamic list
    status = emptySegTaskSLFP(&segmentedTask);
    if(!rtems_is_status_successful(status)) {
        return status;
    }

    status = fillDataIntoSegTaskSLFP(&segmentedTask, taskName, taskStackSize, initialModes,
                taskAttributes, numberOfSegments, segmentFunctions, segmentPriorities);
    if(!rtems_is_status_successful(status)) {
        return status;
    }

    status = rtems_task_create(taskName, segmentedTask.base.taskPriority, taskStackSize, initialModes, taskAttributes, taskId);
    if(!rtems_is_status_successful(status)) {
        return status;
    }

    ((Segmented_Task_Task*)(&segmentedTask))->taskId = *taskId;

    return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_task_start_segmented_slfp(rtems_id taskId) {
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
    return rtems_task_start(base->taskId, main, (rtems_task_argument) NULL);
}

rtems_status_code rtems_task_resume_segmented_slfp(rtems_id taskId) {
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

    *priority = task->priorities[segmentIndex];
    return RTEMS_SUCCESSFUL;
}

rtems_extended_status_code emptySegTaskSLFP(Segmented_Task_SLFP_Task* givenSegmentedTask) {
    // --- Argument validation ---
    // Arguments that need to be validated will be validated in emptySegmentedTask
    // TODO: Do validation in emptySegmentedTask

    // TODO: A extended status code must be returned by the following and must be forwarded. Remember to change the return description.
    emptySegmentedTask((Segmented_Task_Task*) givenSegmentedTask);

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
    // Arguments that need to be validated will be validated in fillDataIntoSegTask
    // TODO: Do validation in fillDataIntoSegTask

    // TODO: A extended status code must be returned by the following and must be forwarded. Remember to change the return description.
    fillDataIntoSegTask((Segmented_Task_Task*) task, taskName, priorities[0], taskStackSize, initialModes, taskAttributes, numberOfSegments, functionPointer);

    for(uint32_t i = 0; i < numberOfSegments; i++) {
        (task->priorities)[i] = priorities[i];
    }

    return RTEMS_SUCCESSFUL;
}

void main(rtems_task_argument arguments) {
    
    Segmented_Task_SLFP_Task* segmentedTask;
    rtems_extended_status_code status = getSegmented_Task_SLFP_Task(RTEMS_SELF, &segmentedTask);
    if(!rtems_is_status_successful(status)) {
        /*
        TODO: What todo when status is not RTEMS_SUCCESSFUL? The main function of the task
        must be exited but with failure.
        */
    }

    Segmented_Task_Task* base = (Segmented_Task_Task*) segmentedTask;
    for(uint32_t i = 0; i < base->numberOfSegments; i++) {
        //base->segments[i].function();
        executeNextSegment(base);
        
        /*
        No suspension nor priority change needed after
        the last segment
        */
        if(i == base->numberOfSegments - 1) {
            break;
        }

        rtems_task_suspend(RTEMS_SELF);
        /*
        Change priority of the task to the priority of the next segment.
        Exact place to apply the change is unknown yet. Temporarly it's
        done after the task is unsuspended.
        */
        base->taskPriority = segmentedTask->priorities[i+1];
        rtems_task_priority wildCard;
        rtems_task_set_priority(RTEMS_SELF, base->taskPriority,&wildCard);
    }
    rtems_task_exit();
}