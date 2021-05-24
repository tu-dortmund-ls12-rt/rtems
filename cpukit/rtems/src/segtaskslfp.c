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
                size_t taskStackSize, rtems_mode initialModes, rtems_attribute taskAttributes) {
    rtems_status_code status;
    rtems_id id;

    Segmented_Task_SLFP_Task segmentedTask;

    fillAssociatedDataStructure(&segmentedTask, taskName, taskPriority, taskStackSize, initialModes, taskAttributes);

    status = rtems_task_create(taskName, taskPriority, taskStackSize, initialModes, taskAttributes, &id);

    return status;
}

bool fillAssociatedDataStructure(Segmented_Task_SLFP_Task* task, rtems_name taskName, rtems_task_priority taskPriority,
                size_t taskStackSize, rtems_mode initialModes, rtems_attribute taskAttributes) {
    fillAssociatedDataStructureGeneral((Segmented_Task_Task*) task, taskName, taskPriority, taskStackSize, initialModes, taskAttributes);
    // TODO: Special Filling
    return true;
}

bool fillAssociatedDataStructureGeneral(Segmented_Task_Task* task, rtems_name taskName, rtems_task_priority taskPriority,
                size_t taskStackSize, rtems_mode initialModes, rtems_attribute taskAttributes) {
    task->taskName = taskName;
    task->taskPriority = taskPriority;
    task->taskStackSize = taskStackSize;
    task->taskModes = initialModes;
    task->taskAttributes = taskAttributes;
    
    return true;
}