#include <rtems/rtems/segtaskslfp.h>
#include <rtems/rtems/segtaskslfpimpl.h>

/*
TaskName -> From the user
TaskPriority -> The Priority of the first segment. TODO: Remove and get from list of segment priorities.
TaskStackSize -> From the user
TaskModes -> From the user
TaskAttributes -> From the user
*/
rtems_status_code rtems_task_create_segmented_slfp(rtems_name taskName, rtems_task_priority taskPriority,
                size_t taskStackSize, rtems_mode initialModes, rtems_attribute taskAttributes, uint32_t numberOfSegments,
                void (*segmentFunctions[]) (void), rtems_task_priority segmentPriorities[], Segmented_Task_SLFP_Task* taskCopy) {
    rtems_status_code status;
    rtems_id id;
    
    emptySegTaskSLFP();
    fillDataIntoSegTaskSLFP(&segmentedTask, taskName, taskStackSize, initialModes,
                taskAttributes, numberOfSegments, segmentFunctions, segmentPriorities);

    status = rtems_task_create(taskName, taskPriority, taskStackSize, initialModes, taskAttributes, &id);

    if(status == RTEMS_SUCCESSFUL) {
        ((Segmented_Task_Task*)(&segmentedTask))->taskId = id;
        *taskCopy = segmentedTask;
    }

    return status;
}

rtems_status_code rtems_task_start_segmented_slfp(/*which task to start*/void) {
    Segmented_Task_Task* base = (Segmented_Task_Task*) &segmentedTask;
    return rtems_task_start(base->taskId, main, (rtems_task_argument) NULL);
}

void emptySegTaskSLFP(void) {
    Segmented_Task_Task* base = (Segmented_Task_Task*) &segmentedTask;
    base->taskName = 0;
    base->taskPriority = 0;
    base->taskStackSize = 0;
    base->taskModes = 0;
    base->taskAttributes = 0;
    base->numberOfSegments = 0;
    
    for(uint32_t i = 0; i < CONFIGURE_MAXIMUM_SEGMENTS; i++) {
        base->segments[i].function = 0;
        segmentedTask.priorities[i] = 0;
    }
}

void fillDataIntoSegTaskSLFP(Segmented_Task_SLFP_Task* task,
                rtems_name taskName, size_t taskStackSize,
                rtems_mode initialModes, rtems_attribute taskAttributes,
                uint32_t numberOfSegments, void (*functionPointer[]) (void),
                rtems_task_priority priorities[]) {
    fillDataIntoSegTask((Segmented_Task_Task*)task, taskName, priorities[0], taskStackSize, initialModes, taskAttributes, numberOfSegments, functionPointer);

    for(uint32_t i = 0; i < numberOfSegments; i++) {
        (task->priorities)[i] = priorities[i];
    }
}

void main(rtems_task_argument arguments) {
    Segmented_Task_Task* base = (Segmented_Task_Task*) &segmentedTask;
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
        base->taskPriority = segmentedTask.priorities[i+1];
        rtems_task_priority wildCard;
        rtems_task_set_priority(RTEMS_SELF, base->taskPriority,&wildCard);
    }
    rtems_task_exit();
}