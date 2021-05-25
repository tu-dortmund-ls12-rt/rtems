#include <rtems/rtems/segtask.h>
#include <rtems/rtems/segtaskimpl.h>

// ----- RTEMS API Implementation -----

rtems_status_code rtems_task_segmented_get_communication_memory_size(Segmented_Task_Task* segmentedTask, size_t* size) {
    *size = CONFIGURE_MAXIMUM_COMMUNICATION_MEMORY;
}

rtems_status_code rtems_task_segmented_write_communication_memory(Segmented_Task_Task* segmentedTask, uint8_t* content, size_t sizeOfContent) {
    if(sizeOfContent > CONFIGURE_MAXIMUM_COMMUNICATION_MEMORY) {
        return RTEMS_INVALID_SIZE;
    }

    if(DO_ZERO_ON_WRITE == 1) {
        rtems_task_segmented_clear_communication_memory(segmentedTask);
    }

    /*
    Just assigning the communicationMemory = content isn't enough, because the content can get out of scope.
    The informations need to be copied by value. *communicationMemory = *content is also not sufficient,
    because it's an uint8_t pointer and therefor only 1 Byte will be copied. Normally memcpy should be used,
    but unsure how it's done in RTEMS, so therefor copy Byte for Byte.

    TODO: Find a more efficient way like memcpy.
    */
    for(uint32_t i = 0; i < sizeOfContent; i++) {
        segmentedTask->communicationMemory[i] = content[i];
    }

    return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_task_segmented_append_communication_memory(Segmented_Task_Task* segmentedTask, uint8_t* content, size_t sizeOfContent) {
    int32_t freeMemory = CONFIGURE_MAXIMUM_COMMUNICATION_MEMORY - segmentedTask->nextByteToWrite;

    /*
    Covers both cases that either the memory is full aready or
    that more memory is needed than what's left.
    */
    if(freeMemory <= sizeOfContent) {
        return RTEMS_INVALID_SIZE;
    }

    for(uint32_t i = 0; i < sizeOfContent; i++) {
        segmentedTask->communicationMemory[i + segmentedTask->nextByteToWrite] = content[i];
    }

    return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_task_segmented_clear_communication_memory(Segmented_Task_Task* segmentedTask) {
    for(uint32_t i = 0; i < CONFIGURE_MAXIMUM_COMMUNICATION_MEMORY; i++) {
        segmentedTask->communicationMemory[i] = 0;
    }

    segmentedTask->nextByteToWrite = 0;

    return RTEMS_SUCCESSFUL;
}

// ----- Hidden Implementation -----

void emptySegmentedTask(Segmented_Task_Task* task) {
    rtems_status_code status;

    task->taskName = 0;
    task->taskPriority = 0;
    task->taskStackSize = 0;
    task->taskModes = 0;
    task->taskAttributes = 0;
    task->numberOfSegments = 0;
    
    for(uint32_t i = 0; i < CONFIGURE_MAXIMUM_SEGMENTS; i++) {
        task->segments[i].function = 0;
    }

    status = rtems_task_segmented_clear_communication_memory(task);
    //TODO: Handle the internal status eror. directive_failed is not available here.
}

void fillDataIntoSegTask(Segmented_Task_Task* task,
                rtems_name taskName, rtems_task_priority taskPriority,
                size_t taskStackSize, rtems_mode initialModes, 
                rtems_attribute taskAttributes, uint32_t numberOfSegments,
                void (*functionPointer[]) (void)) {
    emptySegmentedTask(task);
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