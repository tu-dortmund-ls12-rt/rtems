#include <rtems/rtems/segtaskslfp.h>
#include <rtems/rtems/segtaskslfpimpl.h>

// ----- Global variable declaration / definition -----

Segmented_Task_SLFP_Task segmentedTask; // Currently only one segmented task is possible. Should be replaced with a dynamic data structure.

// ----- RTEMS API Implementation -----

rtems_status_code rtems_task_create_segmented_slfp(rtems_name taskName, size_t taskStackSize,
                rtems_mode initialModes, rtems_attribute taskAttributes, uint32_t numberOfSegments,
                void (*segmentFunctions[]) (void), rtems_task_priority segmentPriorities[], rtems_id* taskId) {
    rtems_status_code status;
    
    //TODO: Ditch the single task, and create a new one in a dynamic list
    emptySegTaskSLFP(&segmentedTask);
    fillDataIntoSegTaskSLFP(&segmentedTask, taskName, taskStackSize, initialModes,
                taskAttributes, numberOfSegments, segmentFunctions, segmentPriorities);

    status = rtems_task_create(taskName, segmentedTask.base.taskPriority, taskStackSize, initialModes, taskAttributes, taskId);

    if(status == RTEMS_SUCCESSFUL) {
        ((Segmented_Task_Task*)(&segmentedTask))->taskId = *taskId;
    }

    return status;
}

rtems_status_code rtems_task_start_segmented_slfp(rtems_id taskId) {
    /*
    Somehow redundant, but may be usefull in the future to check if taskId
    is associated with a segmented SLFP task.
    */
    Segmented_Task_SLFP_Task* receivedSegmentedTask;
    bool result = getSegmented_Task_SLFP_Task(taskId, &receivedSegmentedTask);
    Segmented_Task_Task* base = (Segmented_Task_Task*) receivedSegmentedTask;
    return rtems_task_start(base->taskId, main, (rtems_task_argument) NULL);
}

// ----- Hidden Implementation -----

bool getSegmented_Task_SLFP_Task(rtems_id id, Segmented_Task_SLFP_Task** segmentedTaskToReturn) {
    //TODO: Implement going through the list of all slfp tasks
    *segmentedTaskToReturn = &segmentedTask;

    return true;
}

void emptySegTaskSLFP(Segmented_Task_SLFP_Task* givenSegmentedTask) {
    emptySegmentedTask((Segmented_Task_Task*) givenSegmentedTask);

    for(uint32_t i = 0; i < CONFIGURE_MAXIMUM_SEGMENTS; i++) {
        givenSegmentedTask->priorities[i] = 0;
    }
}

void fillDataIntoSegTaskSLFP(Segmented_Task_SLFP_Task* task,
                rtems_name taskName, size_t taskStackSize,
                rtems_mode initialModes, rtems_attribute taskAttributes,
                uint32_t numberOfSegments, void (*functionPointer[]) (void),
                rtems_task_priority priorities[]) {
    fillDataIntoSegTask((Segmented_Task_Task*) task, taskName, priorities[0], taskStackSize, initialModes, taskAttributes, numberOfSegments, functionPointer);

    for(uint32_t i = 0; i < numberOfSegments; i++) {
        (task->priorities)[i] = priorities[i];
    }
}

void main(rtems_task_argument arguments) {
    
    Segmented_Task_SLFP_Task* segmentedTask;
    bool result = getSegmented_Task_SLFP_Task(RTEMS_SELF, &segmentedTask);
    Segmented_Task_Task* base = (Segmented_Task_Task*) segmentedTask;
    for(uint32_t i = 0; i < base->numberOfSegments; i++) {
        base->segments[i].function();
        
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