#include <rtems/rtems/segtask.h>
#include <rtems/rtems/segtaskimpl.h>

// ----- RTEMS API Implementation -----

rtems_status_code rtems_task_segmented_get_communication_memory_size(size_t* size) {
    Segmented_Task_Task* correspondingTask = NULL;
    bool result = getSegmented_Task_Task(RTEMS_SELF, &correspondingTask);

    if(result == false || correspondingTask == NULL) {
        return RTEMS_NOT_OWNER_OF_RESOURCE; // TODO: Replace with correct error code
    }

    return rtems_task_segmented_get_communication_memory_size_impl(correspondingTask, size);
}

rtems_status_code rtems_task_segmented_read_communication_memory_for_from(uint8_t* buffer, size_t bufferSize,
                                                                        size_t amount, uint32_t startingAtByte) {
    Segmented_Task_Task* correspondingTask = NULL;
    bool result = getSegmented_Task_Task(RTEMS_SELF, &correspondingTask);

    if(result == false || correspondingTask == NULL) {
        return RTEMS_NOT_OWNER_OF_RESOURCE; // TODO: Replace with correct error code
    }

    return rtems_task_segmented_read_communication_memory_for_from_impl(correspondingTask, buffer, bufferSize, amount, startingAtByte);
}

rtems_status_code rtems_task_segmented_read_communication_memory_for(uint8_t* buffer, size_t bufferSize,
                                                                        size_t amount) {
    Segmented_Task_Task* correspondingTask = NULL;
    bool result = getSegmented_Task_Task(RTEMS_SELF, &correspondingTask);

    if(result == false || correspondingTask == NULL) {
        return RTEMS_NOT_OWNER_OF_RESOURCE; // TODO: Replace with correct error code
    }

    return rtems_task_segmented_read_communication_memory_for_impl(correspondingTask, buffer, bufferSize, amount);
}

rtems_status_code rtems_task_segmented_read_communication_memory_from(uint8_t* buffer, size_t bufferSize,
                                                                        uint32_t startingAtByte) {
    Segmented_Task_Task* correspondingTask = NULL;
    bool result = getSegmented_Task_Task(RTEMS_SELF, &correspondingTask);

    if(result == false || correspondingTask == NULL) {
        return RTEMS_NOT_OWNER_OF_RESOURCE; // TODO: Replace with correct error code
    }

    return rtems_task_segmented_read_communication_memory_from_impl(correspondingTask, buffer, bufferSize, startingAtByte);
}

rtems_status_code rtems_task_segmented_read_communication_memory(uint8_t* buffer, size_t bufferSize) {
    Segmented_Task_Task* correspondingTask = NULL;
    bool result = getSegmented_Task_Task(RTEMS_SELF, &correspondingTask);

    if(result == false || correspondingTask == NULL) {
        return RTEMS_NOT_OWNER_OF_RESOURCE; // TODO: Replace with correct error code
    }

    return rtems_task_segmented_read_communication_memory_impl(correspondingTask, buffer, bufferSize);
}

rtems_status_code rtems_task_segmented_write_communication_memory(uint8_t* content, size_t sizeOfContent) {
    Segmented_Task_Task* correspondingTask = NULL;
    bool result = getSegmented_Task_Task(RTEMS_SELF, &correspondingTask);

    if(result == false || correspondingTask == NULL) {
        return RTEMS_NOT_OWNER_OF_RESOURCE; // TODO: Replace with correct error code
    }

    return rtems_task_segmented_write_communication_memory_impl(correspondingTask, content, sizeOfContent);
}

rtems_status_code rtems_task_segmented_append_communication_memory(uint8_t* content, size_t sizeOfContent) {
    Segmented_Task_Task* correspondingTask = NULL;
    bool result = getSegmented_Task_Task(RTEMS_SELF, &correspondingTask);

    if(result == false || correspondingTask == NULL) {
        return RTEMS_NOT_OWNER_OF_RESOURCE; // TODO: Replace with correct error code
    }

    return rtems_task_segmented_append_communication_memory_impl(correspondingTask, content, sizeOfContent);
}

rtems_status_code rtems_task_segmented_clear_communication_memory(void) {
    Segmented_Task_Task* correspondingTask = NULL;
    bool result = getSegmented_Task_Task(RTEMS_SELF, &correspondingTask);

    if(result == false || correspondingTask == NULL) {
        return RTEMS_NOT_OWNER_OF_RESOURCE; // TODO: Replace with correct error code
    }

    return rtems_task_segmented_clear_communication_memory_impl(correspondingTask);
}

// ----- Hidden Implementation -----

void executeNextSegment(Segmented_Task_Task* task) {
    /*
    TODO: What to do if a segment should be executed that is not present?
    */
    task->segments[++task->currentSegment].function();
}

void emptySegmentedTask(Segmented_Task_Task* task) {
    rtems_status_code status;

    task->taskName = 0;
    task->taskPriority = 0;
    task->taskStackSize = 0;
    task->taskModes = 0;
    task->taskAttributes = 0;
    task->numberOfSegments = 0;
    task->currentSegment = -1;
    
    for(uint32_t i = 0; i < CONFIGURE_MAXIMUM_SEGMENTS; i++) {
        task->segments[i].function = 0;
    }

    status = rtems_task_segmented_clear_communication_memory_impl(task);
    //TODO: Handle the internal status eror. directive_failed is not available here.
}

bool getSegmented_Task_Task(rtems_id id, Segmented_Task_Task** segmentedTaskToReturn) {
    Segmented_Task_SLFP_Task* receivedTask1 = NULL;
    bool result = getSegmented_Task_SLFP_Task(id, &receivedTask1);
    if(result) {
        *segmentedTaskToReturn = (Segmented_Task_Task*) receivedTask1;
        return true;
    }

    // TODO: Same for Realtime_Enforcement

    //Else
    return false; // The given id does not coresspond with a SLFP or Realtime Enforcement Task
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

rtems_status_code rtems_task_segmented_get_communication_memory_size_impl(Segmented_Task_Task* segmentedTask, size_t* size) {
    /*
    TODO:
    So far each segmented task has the same sized communication memory. The result is therefor indepented of the given
    segmented task.
    */
    *size = CONFIGURE_MAXIMUM_COMMUNICATION_MEMORY;
}

rtems_status_code rtems_task_segmented_read_communication_memory_for_from_impl(Segmented_Task_Task* segmentedTask, uint8_t* buffer, size_t bufferSize,
                                                                        size_t amount, uint32_t startingAtByte) {
    if(bufferSize < amount) {
        return RTEMS_INVALID_SIZE;
    }

    if(startingAtByte + amount > CONFIGURE_MAXIMUM_COMMUNICATION_MEMORY) {
        return RTEMS_INVALID_SIZE;
    }

    for(uint32_t i = 0; i < amount; i++) {
        buffer[i] = segmentedTask->communicationMemory[i + startingAtByte];
    }

    return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_task_segmented_read_communication_memory_for_impl(Segmented_Task_Task* segmentedTask, uint8_t* buffer, size_t bufferSize,
                                                                        size_t amount) {
    return rtems_task_segmented_read_communication_memory_for_from_impl(segmentedTask, buffer, bufferSize, amount, 0);
}

rtems_status_code rtems_task_segmented_read_communication_memory_from_impl(Segmented_Task_Task* segmentedTask, uint8_t* buffer, size_t bufferSize,
                                                                        uint32_t startingAtByte) {
    return rtems_task_segmented_read_communication_memory_for_from_impl(segmentedTask, buffer, bufferSize,
                                                                    CONFIGURE_MAXIMUM_COMMUNICATION_MEMORY - startingAtByte, startingAtByte);
}

rtems_status_code rtems_task_segmented_read_communication_memory_impl(Segmented_Task_Task* segmentedTask, uint8_t* buffer, size_t bufferSize) {
    return rtems_task_segmented_read_communication_memory_from_impl(segmentedTask, buffer, bufferSize, 0);
}

rtems_status_code rtems_task_segmented_write_communication_memory_impl(Segmented_Task_Task* segmentedTask, uint8_t* content, size_t sizeOfContent) {
    if(sizeOfContent > CONFIGURE_MAXIMUM_COMMUNICATION_MEMORY) {
        return RTEMS_INVALID_SIZE;
    }

    if(DO_ZERO_ON_WRITE == 1) {
        rtems_task_segmented_clear_communication_memory_impl(segmentedTask);
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

rtems_status_code rtems_task_segmented_append_communication_memory_impl(Segmented_Task_Task* segmentedTask, uint8_t* content, size_t sizeOfContent) {
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

rtems_status_code rtems_task_segmented_clear_communication_memory_impl(Segmented_Task_Task* segmentedTask) {
    for(uint32_t i = 0; i < CONFIGURE_MAXIMUM_COMMUNICATION_MEMORY; i++) {
        segmentedTask->communicationMemory[i] = 0;
    }

    segmentedTask->nextByteToWrite = 0;

    return RTEMS_SUCCESSFUL;
}