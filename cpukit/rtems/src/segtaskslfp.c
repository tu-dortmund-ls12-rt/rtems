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

    Segmented_Task_SLFP_Task segmentedTask;

    fillDataIntoSegTaskSLFP(&segmentedTask, taskName, taskStackSize, initialModes,
                taskAttributes, numberOfSegments, segmentFunctions, segmentPriorities);

    status = rtems_task_create(taskName, taskPriority, taskStackSize, initialModes, taskAttributes, &id);

    if(status == RTEMS_SUCCESSFUL) {
        ((Segmented_Task_Task*)(&segmentedTask))->taskId = id;
        printf("New TRY!!!!!!!\n-----------\n");
        printf("Priority should be 3: %u\n", segmentedTask.priorities[0]);
        printf("Priority of Copy should be random: %u\n", (taskCopy->priorities)[0]);
        *taskCopy = segmentedTask;
        printf("After the copy!!!!!!!\n-----------\n");
        printf("Priority should be 3: %u\n", segmentedTask.priorities[0]);
        printf("Priority of Copy should be random: %u\n", (taskCopy->priorities)[0]);
        printf("TaskCopy->Priority Adress: %p", taskCopy->priorities);
    }

    return status;
}

void fillDataIntoSegTaskSLFP(Segmented_Task_SLFP_Task* task,
                rtems_name taskName, size_t taskStackSize,
                rtems_mode initialModes, rtems_attribute taskAttributes,
                uint32_t numberOfSegments, void (*functionPointer[]) (void),
                rtems_task_priority priorities[]) {
    fillDataIntoSegTask((Segmented_Task_Task*)task, taskName, priorities[0], taskStackSize, initialModes, taskAttributes, numberOfSegments, functionPointer);

    for(uint32_t i = 0; i < numberOfSegments; i++) {
        printf("New Loop\n");
        printf("Task->Priorities[%u]: %u\n", i, (task->priorities)[i]);
        printf("Priorities[%u]: %u\n", i, priorities[i]);
        (task->priorities)[i] = priorities[i];
        printf("After Assignment\n");
        printf("Task->Priorities[%u]: %u\n", i, (task->priorities)[i]);
    }

    printf("Prioritites were filled!\n");
}