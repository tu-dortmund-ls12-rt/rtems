#include <rtems/rtems/segtaskimpl.h>

void fillDataIntoSegTask(Segmented_Task_Task* task,
                rtems_name taskName, rtems_task_priority taskPriority,
                size_t taskStackSize, rtems_mode initialModes, 
                rtems_attribute taskAttributes, uint32_t numberOfSegments,
                void (*functionPointer[]) (void)) {
    fillGeneralDataIntoSegTask(task, taskName, taskPriority, taskStackSize, initialModes, taskAttributes);
    fillSegmentDataIntoSegTask(task, numberOfSegments, functionPointer);
}

void fillGeneralDataIntoSegTask(Segmented_Task_Task* task,
                rtems_name taskName, rtems_task_priority taskPriority,
                size_t taskStackSize, rtems_mode initialModes, 
                rtems_attribute taskAttributes) {
    task->taskName = taskName;
    task->taskPriority = taskPriority;
    task->taskStackSize = taskStackSize;
    task->taskModes = initialModes;
    task->taskAttributes = taskAttributes;
}

void fillSegmentDataIntoSegTask(Segmented_Task_Task* task,
                uint32_t numberOfSegments, void (*functionPointer[]) (void)) {
    task->numberOfSegments = numberOfSegments;
    
    for(uint32_t i = 0; i < numberOfSegments; i++) {
        task->segments[i].function = functionPointer[i];
    }
}