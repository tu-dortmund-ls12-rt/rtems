#include <rtems/rtems/segtask.h>
#include <rtems/rtems/segtaskimpl.h>

// ----- RTEMS API Implementation -----

rtems_status_code rtems_task_segmented_get_communication_memory_size(size_t* size) {
    // --- Argument Validation
    if(size == NULL) {
        return RTEMS_EXTENDED_NULL_POINTER;
    }

    // --- Implementation ---
    *size = CONFIGURE_MAXIMUM_COMMUNICATION_MEMORY;
    return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_task_segmented_read_communication_memory_for_from(uint8_t* buffer, size_t bufferSize,
                                                                        size_t amount, uint32_t startingAtByte) {
    // --- Argument Validation
    /**
     * Arguments that need to be validated will be validated in
     * rtems_task_segmented_read_communication_memory_for_from_impl due to optimization.
     */

    // --- Implementation ---
    Segmented_Task_Task* correspondingTask = NULL;
    rtems_extended_status_code status;
    rtems_id ownId;
    
    status = rtems_task_ident(RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &ownId);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_INVALID_ADDRESS:
         *      Not user dependent. The cause must therefor be internal.
         * RTEMS_INVALID_NAME:
         *      Not user dependent. The cause must therefor be internal.
         * RTEMS_INVALID_NODE:
         *      Not user dependent. The cause must therefor be internal.
         */
        return RTEMS_INTERNAL_ERROR;
    }
    
    status = getSegmented_Task_Task(ownId, &correspondingTask);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_INVALID_ID:
         *      Not user dependent. The cause must therefor be internal.
         * RTEMS_EXTENDED_NULL_POINTER:
         *      Not user dependent. The cause must therefor be internal.
         * RTEMS_INTERNAL_ERROR:
         *      Not user dependent. The cause must therefor be internal.
         */
        return RTEMS_INTERNAL_ERROR;
    }

    status = rtems_task_segmented_read_communication_memory_for_from_impl(correspondingTask, buffer, bufferSize, amount, startingAtByte);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_EXTENDED_NULL_POINTER:
         *      User independent correspondingTask null pointer would have been catched beforehand.
         *      Therefor the buffer pointer must be null.
         * RTEMS_INVALID_SIZE:
         *      The buffer is to small or a read outside the boundarys should be performed. Both
         *      cases are user dependent.
         */
        return status;
    }

    return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_task_segmented_read_communication_memory_for(uint8_t* buffer, size_t bufferSize,
                                                                        size_t amount) {
    // --- Argument Validation
    /**
     * Arguments that need to be validated will be validated in
     * rtems_task_segmented_read_communication_memory_for_impl due to optimization.
     */

    // --- Implementation ---
    Segmented_Task_Task* correspondingTask = NULL;
    rtems_extended_status_code status;
    rtems_id ownId;
    
    status = rtems_task_ident(RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &ownId);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_INVALID_ADDRESS:
         *      Not user dependent. The cause must therefor be internal.
         * RTEMS_INVALID_NAME:
         *      Not user dependent. The cause must therefor be internal.
         * RTEMS_INVALID_NODE:
         *      Not user dependent. The cause must therefor be internal.
         */
        return RTEMS_INTERNAL_ERROR;
    }
    
    status = getSegmented_Task_Task(ownId, &correspondingTask);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_INVALID_ID:
         *      Not user dependent. The cause must therefor be internal.
         * RTEMS_EXTENDED_NULL_POINTER:
         *      Not user dependent. The cause must therefor be internal.
         * RTEMS_INTERNAL_ERROR:
         *      Not user dependent. The cause must therefor be internal.
         */
        return RTEMS_INTERNAL_ERROR;
    }

    status = rtems_task_segmented_read_communication_memory_for_impl(correspondingTask, buffer, bufferSize, amount);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_EXTENDED_NULL_POINTER:
         *      User independent correspondingTask null pointer would have been catched beforehand.
         *      Therefor the buffer pointer must be null.
         * RTEMS_INVALID_SIZE:
         *      The buffer is to small or a read outside the boundarys should be performed. Both
         *      cases are user dependent.
         */
        return status;
    }

    return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_task_segmented_read_communication_memory_from(uint8_t* buffer, size_t bufferSize,
                                                                        uint32_t startingAtByte) {
    // --- Argument Validation
    /**
     * Arguments that need to be validated will be validated in
     * rtems_task_segmented_read_communication_memory_from_impl due to optimization.
     */

    // --- Implementation ---
    Segmented_Task_Task* correspondingTask = NULL;
    rtems_extended_status_code status;
    rtems_id ownId;

    status = rtems_task_ident(RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &ownId);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_INVALID_ADDRESS:
         *      Not user dependent. The cause must therefor be internal.
         * RTEMS_INVALID_NAME:
         *      Not user dependent. The cause must therefor be internal.
         * RTEMS_INVALID_NODE:
         *      Not user dependent. The cause must therefor be internal.
         */
        return RTEMS_INTERNAL_ERROR;
    }
    
    status = getSegmented_Task_Task(ownId, &correspondingTask);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_INVALID_ID:
         *      Not user dependent. The cause must therefor be internal.
         * RTEMS_EXTENDED_NULL_POINTER:
         *      Not user dependent. The cause must therefor be internal.
         * RTEMS_INTERNAL_ERROR:
         *      Not user dependent. The cause must therefor be internal.
         */
        return RTEMS_INTERNAL_ERROR;
    }

    status = rtems_task_segmented_read_communication_memory_from_impl(correspondingTask, buffer, bufferSize, startingAtByte);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_EXTENDED_NULL_POINTER:
         *      User independent correspondingTask null pointer would have been catched beforehand.
         *      Therefor the buffer pointer must be null.
         * RTEMS_INVALID_SIZE:
         *      The buffer is to small or a read outside the boundarys should be performed. Both
         *      cases are user dependent.
         */
        return status;
    }

    return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_task_segmented_read_communication_memory(uint8_t* buffer, size_t bufferSize) {
    // --- Argument Validation
    /**
     * Arguments that need to be validated will be validated in
     * rtems_task_segmented_read_communication_memory_from_impl due to optimization.
     */

    // --- Implementation ---
    Segmented_Task_Task* correspondingTask = NULL;
    rtems_extended_status_code status;
    rtems_id ownId;
    
    status = rtems_task_ident(RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &ownId);
    if(!rtems_is_status_successful(status)) {
        return status; // TODO: REAL ERROR HANDLING LIKE IN THE OTHER CASES. ALSO EXTEND DOC.
    }
    
    status = getSegmented_Task_Task(ownId, &correspondingTask);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_INVALID_ID:
         *      Not user dependent. The cause must therefor be internal.
         * RTEMS_EXTENDED_NULL_POINTER:
         *      Not user dependent. The cause must therefor be internal.
         * RTEMS_INTERNAL_ERROR:
         *      Not user dependent. The cause must therefor be internal.
         */
        return RTEMS_INTERNAL_ERROR;
    }

    status = rtems_task_segmented_read_communication_memory_impl(correspondingTask, buffer, bufferSize);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_EXTENDED_NULL_POINTER:
         *      User independent correspondingTask null pointer would have been catched beforehand.
         *      Therefor the buffer pointer must be null.
         * RTEMS_INVALID_SIZE:
         *      The buffer is to small or a read outside the boundarys should be performed. Both
         *      cases are user dependent.
         */
        return status;
    }

    return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_task_segmented_write_communication_memory(uint8_t* content, size_t sizeOfContent) {
    // --- Argument Validation
    /**
     * Arguments that need to be validated will be validated in
     * rtems_task_segmented_read_communication_memory_from_impl due to optimization.
     */

    // --- Implementation ---
    Segmented_Task_Task* correspondingTask = NULL;
    rtems_extended_status_code status;
    rtems_id ownId;
    
    status = rtems_task_ident(RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &ownId);
    if(!rtems_is_status_successful(status)) {
        return status; // TODO: REAL ERROR HANDLING LIKE IN THE OTHER CASES. ALSO EXTEND DOC.
    }
    
    status = getSegmented_Task_Task(ownId, &correspondingTask);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_INVALID_ID:
         *      Not user dependent. The cause must therefor be internal.
         * RTEMS_EXTENDED_NULL_POINTER:
         *      Not user dependent. The cause must therefor be internal.
         * RTEMS_INTERNAL_ERROR:
         *      Not user dependent. The cause must therefor be internal.
         */
        return RTEMS_INTERNAL_ERROR;
    }

    status = rtems_task_segmented_write_communication_memory_impl(correspondingTask, content, sizeOfContent);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_EXTENDED_NULL_POINTER:
         *      User independent correspondingTask null pointer would have been catched beforehand.
         *      Therefor the content pointer must be null.
         * RTEMS_INVALID_SIZE:
         *      sizeOfContent is zero or not enough communication memory is present to write the whole content.
         * RTEMS_INTERNAL_ERROR:
         *      Not user dependet. The internal error must be forwarded.
         */
        return status;
    }

    return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_task_segmented_append_communication_memory(uint8_t* content, size_t sizeOfContent) {
    // --- Argument Validation
    /**
     * Arguments that need to be validated will be validated in
     * rtems_task_segmented_read_communication_memory_from_impl due to optimization.
     */

    // --- Implementation ---
    Segmented_Task_Task* correspondingTask = NULL;
    rtems_extended_status_code status;
    rtems_id ownId;
    
    status = rtems_task_ident(RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &ownId);
    if(!rtems_is_status_successful(status)) {
        return status; // TODO: REAL ERROR HANDLING LIKE IN THE OTHER CASES. ALSO EXTEND DOC.
    }
    
    status = getSegmented_Task_Task(ownId, &correspondingTask);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_INVALID_ID:
         *      Not user dependent. The cause must therefor be internal.
         * RTEMS_EXTENDED_NULL_POINTER:
         *      Not user dependent. The cause must therefor be internal.
         * RTEMS_INTERNAL_ERROR:
         *      Not user dependent. The cause must therefor be internal.
         */
        return RTEMS_INTERNAL_ERROR;
    }

    status = rtems_task_segmented_append_communication_memory_impl(correspondingTask, content, sizeOfContent);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_EXTENDED_NULL_POINTER:
         *      User independent correspondingTask null pointer would have been catched beforehand.
         *      Therefor the content pointer must be null.
         * RTEMS_INVALID_SIZE:
         *      sizeOfContent is zero or not enough communication memory is left to write the whole content.
         */
        return status;
    }

    return RTEMS_SUCCESSFUL;
}

rtems_status_code rtems_task_segmented_clear_communication_memory(void) {
    // --- Argument Validation
    /**
     * Arguments that need to be validated will be validated in
     * rtems_task_segmented_read_communication_memory_from_impl due to optimization.
     */

    // --- Implementation ---
    Segmented_Task_Task* correspondingTask = NULL;
    rtems_extended_status_code status;
    rtems_id ownId;

    status = rtems_task_ident(RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &ownId);
    if(!rtems_is_status_successful(status)) {
        return status; // TODO: REAL ERROR HANDLING LIKE IN THE OTHER CASES. ALSO EXTEND DOC.
    }
    
    status = getSegmented_Task_Task(ownId, &correspondingTask);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_INVALID_ID:
         *      Not user dependent. The cause must therefor be internal.
         * RTEMS_EXTENDED_NULL_POINTER:
         *      Not user dependent. The cause must therefor be internal.
         * RTEMS_INTERNAL_ERROR:
         *      Not user dependent. The cause must therefor be internal.
         */
        return RTEMS_INTERNAL_ERROR;
    }

    status = rtems_task_segmented_clear_communication_memory_impl(correspondingTask);
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_EXTENDED_NULL_POINTER:
         *      User independent correspondingTask null pointer would have been catched beforehand.
         *      The cause must therefor be internal.
         */
        return RTEMS_INTERNAL_ERROR;
    }

    return RTEMS_SUCCESSFUL;
}

// ----- Hidden Implementation -----

bool isPriorityValid(rtems_task_priority priority) {
    /*
    The priority range might be changed in the future. Maybe there is another
    way to check the validity of a priority through an existing core call?
    */
    if(priority >= 1 && priority <= 255) {
        return true;
    } else {
        return false;
    }
}

rtems_extended_status_code executeNextSegment(Segmented_Task_Task* task, rtems_task_argument arguments) {
    // --- Argument validation ---
    /**
     * It is not possible to validate arguments. It is in the users
     * responsibility, that those arguments are correct.
     */

    if(task == NULL) {
        return RTEMS_EXTENDED_NULL_POINTER;
    }

    // --- Implementation ---
    // Reached the last segment.
    if(task->currentSegment + 1 == task->numberOfSegments) {
        return RTEMS_EXTENDED_FINAL_SEGMENT;
    }

    (task->currentSegment)++;

    Segmented_Task_Arguments args;
    args.currentSegment = task->currentSegment;
    args.arguments = arguments;

    task->segments[task->currentSegment].function(args);
    return RTEMS_SUCCESSFUL;
}

rtems_extended_status_code emptySegmentedTask(Segmented_Task_Task* task) {
    // --- Argument validation ---
    if(task == NULL) {
        return RTEMS_EXTENDED_NULL_POINTER;
    }

    // --- Implementation ---
    rtems_extended_status_code status;

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
    if(!rtems_is_status_successful(status)) {
        /**
         * Possible errors:
         * RTEMS_EXTENDED_NULL_POINTER:
         *      User depented, but validated beforehand. Should not occur.
         */
        return RTEMS_INTERNAL_ERROR;
    }

    return RTEMS_SUCCESSFUL;
}

rtems_extended_status_code getSegmented_Task_Task(rtems_id id, Segmented_Task_Task** segmentedTaskToReturn) {
    // --- Argument validation ---
    if(segmentedTaskToReturn == NULL) {
        return RTEMS_EXTENDED_NULL_POINTER;
    }

    // --- Implementation ---
    // TODO: Extend commentlike for Realtime Enforcement Tasks
    Segmented_Task_SLFP_Task* slfpTask = NULL;
    // Segmented_Task_RE_Task* reTask = NULL;
    rtems_extended_status_code status;

    status = getSegmented_Task_SLFP_Task(id, &slfpTask);
    if(rtems_is_status_successful(status)) {
        *segmentedTaskToReturn = (Segmented_Task_Task*) slfpTask;
        return RTEMS_SUCCESSFUL;
    } else if((rtems_status_code) status != RTEMS_INVALID_ID) {
        return status;
    } else {
        /**
         * Possible errors:
         * RTEMS_EXTENDED_NULL_POINTER:
         *      User depented, but validated beforehand. Should not occur.
         */
        return RTEMS_INTERNAL_ERROR;
    }

    /*
    status = getSegmented_Task_RE_Task(id, &reTask);
    if(rtems_is_status_successful(status)) {
        *segmentedTaskToReturn = (Segmented_Task_Task*) reTask;
        return RTEMS_SUCCESSFUL;
    } else if(status != RTEMS_INVALID_ID) {
        return status;
    } else {
        /**
         * Possible errors:
         * RTEMS_EXTENDED_NULL_POINTER:
         *      User depented, but validated beforehand. Should not occur.
         */
        /*return RTEMS_INTERNAL_ERROR;
    }
    */
}

rtems_extended_status_code fillDataIntoSegTask(Segmented_Task_Task* task,
                rtems_name taskName, rtems_task_priority taskPriority,
                size_t taskStackSize, rtems_mode initialModes, 
                rtems_attribute taskAttributes, uint32_t numberOfSegments,
                void (*functionPointer[]) (Segmented_Task_Arguments)) {
    // --- Argument validation ---
    /**
     * Arguments that need to be validated will be validated in emptySegmentedTask,
     * fillGeneralDataIntoSegTask and fillSegmentDataIntoSegTask due to optimization.
     */

    // --- Implementation ---
    rtems_extended_status_code status;

    status = emptySegmentedTask(task);
    if(!rtems_is_status_successful(status)) {
        /**
         * Every possible error needs to be forwarded.
         */
        return status;
    }

    status = fillGeneralDataIntoSegTask(task, taskName, taskPriority, taskStackSize, initialModes, taskAttributes);
    if(!rtems_is_status_successful(status)) {
        /**
         * Every possible error needs to be forwarded.
         */
        return status;
    }

    status = fillSegmentDataIntoSegTask(task, numberOfSegments, functionPointer);
    if(!rtems_is_status_successful(status)) {
        /**
         * Every possible error needs to be forwarded.
         */
        return status;
    }

    return RTEMS_SUCCESSFUL;
}

rtems_extended_status_code fillGeneralDataIntoSegTask(Segmented_Task_Task* task,
                rtems_name taskName, rtems_task_priority taskPriority,
                size_t taskStackSize, rtems_mode initialModes, 
                rtems_attribute taskAttributes) {
    // --- Argument validation ---
    /* TODO: Validation of taskStackSize, initialModes and taskAttributes doesn't
    * seem to be possible. Check for further intel.
    * Potential errors will be found during the task creation.
    * If those informations are invalid, the struct will be filled with invalid
    * informations.
    */
    if(task == NULL) {
        return RTEMS_EXTENDED_NULL_POINTER;
    }

    if(!rtems_is_name_valid(taskName)) {
        return RTEMS_INVALID_NAME;
    }

    if(!isPriorityValid(taskPriority)) {
        return RTEMS_INVALID_PRIORITY;
    }

    // --- Implementation ---
    task->taskName = taskName;
    task->taskPriority = taskPriority;
    task->taskStackSize = taskStackSize;
    task->taskModes = initialModes;
    task->taskAttributes = taskAttributes;

    return RTEMS_SUCCESSFUL;
}

rtems_extended_status_code fillSegmentDataIntoSegTask(Segmented_Task_Task* task,
                uint32_t numberOfSegments, void (*functionPointer[]) (Segmented_Task_Arguments)) {
    // --- Argument validation ---
    if(task == NULL) {
        return RTEMS_EXTENDED_NULL_POINTER;
    }

    if(numberOfSegments > CONFIGURE_MAXIMUM_SEGMENTS) {
        return RTEMS_EXTENDED_TOO_MANY_SEGMENTS;
    }

    /**
     * TODO:
     * If optimization is needed, this for loop can be avoided
     * by checking during the assignment.
     */
    for(uint32_t i = 0; i < numberOfSegments; i++) {
        if(functionPointer[i] == NULL) {
            return RTEMS_EXTENDED_NULL_POINTER;
        }
    }

    // --- Implementation ---
    task->numberOfSegments = numberOfSegments;
    
    for(uint32_t i = 0; i < numberOfSegments; i++) {
        task->segments[i].function = functionPointer[i];
    }

    return RTEMS_SUCCESSFUL;
}

rtems_extended_status_code rtems_task_segmented_read_communication_memory_for_from_impl(Segmented_Task_Task* segmentedTask, uint8_t* buffer, size_t bufferSize,
                                                                        size_t amount, uint32_t startingAtByte) {
    // --- Argument validation ---
    if(segmentedTask == NULL || buffer == NULL) {
        return RTEMS_EXTENDED_NULL_POINTER;
    }

    if(bufferSize < amount || startingAtByte + amount > CONFIGURE_MAXIMUM_COMMUNICATION_MEMORY) {
        return RTEMS_INVALID_SIZE;
    }

    // --- Implementation ---
    for(uint32_t i = 0; i < amount; i++) {
        buffer[i] = segmentedTask->communicationMemory[i + startingAtByte];
    }

    return RTEMS_SUCCESSFUL;
}

rtems_extended_status_code rtems_task_segmented_read_communication_memory_for_impl(Segmented_Task_Task* segmentedTask, uint8_t* buffer, size_t bufferSize,
                                                                        size_t amount) {
    // --- Argument validation ---
    /**
     * Arguments that need to be validated will be validated in rtems_task_segmented_read_communication_memory_for_from_impl
     */

    // --- Implementation ---
    rtems_extended_status_code status;

    status = rtems_task_segmented_read_communication_memory_for_from_impl(segmentedTask, buffer, bufferSize, amount, 0);
    if(!rtems_is_status_successful(status)) {
        return status;
    }

    return RTEMS_SUCCESSFUL;
}

rtems_extended_status_code rtems_task_segmented_read_communication_memory_from_impl(Segmented_Task_Task* segmentedTask, uint8_t* buffer, size_t bufferSize,
                                                                        uint32_t startingAtByte) {
    // --- Argument validation ---
    /**
     * Arguments that need to be validated will be validated in rtems_task_segmented_read_communication_memory_for_from_impl
     */

    // --- Implementation ---
    rtems_extended_status_code status;

    status = rtems_task_segmented_read_communication_memory_for_from_impl(segmentedTask, buffer, bufferSize, CONFIGURE_MAXIMUM_COMMUNICATION_MEMORY - startingAtByte,
                                                                        startingAtByte);
    if(!rtems_is_status_successful(status)) {
        return status;
    }

    return RTEMS_SUCCESSFUL;
}

rtems_extended_status_code rtems_task_segmented_read_communication_memory_impl(Segmented_Task_Task* segmentedTask, uint8_t* buffer, size_t bufferSize) {
    // --- Argument validation ---
    /**
     * Arguments that need to be validated will be validated in rtems_task_segmented_read_communication_memory_for_from_impl
     */

    // --- Implementation ---
    rtems_extended_status_code status;

    status = rtems_task_segmented_read_communication_memory_from_impl(segmentedTask, buffer, bufferSize, 0);
    if(!rtems_is_status_successful(status)) {
        return status;
    }

    return RTEMS_SUCCESSFUL;
}

rtems_extended_status_code rtems_task_segmented_write_communication_memory_impl(Segmented_Task_Task* segmentedTask, uint8_t* content, size_t sizeOfContent) {
    // --- Argument validation ---
    if(segmentedTask == NULL || content == NULL) {
        return RTEMS_EXTENDED_NULL_POINTER;
    }

    if(sizeOfContent == 0 || sizeOfContent > CONFIGURE_MAXIMUM_COMMUNICATION_MEMORY) {
        return RTEMS_INVALID_SIZE;
    }

    // --- Implementation ---
    rtems_extended_status_code status;

    if(DO_ZERO_ON_WRITE == 1) {
        status = rtems_task_segmented_clear_communication_memory_impl(segmentedTask);
        if(!rtems_is_status_successful(status)) {
            /**
             * Possible errors:
             * RTEMS_EXTENDED_NULL_POINTER:
             *      User depented, but validated beforehand. Should not occur.
             */
            return RTEMS_INTERNAL_ERROR;
        }
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

rtems_extended_status_code rtems_task_segmented_append_communication_memory_impl(Segmented_Task_Task* segmentedTask, uint8_t* content, size_t sizeOfContent) {
    // --- Argument validation ---
    int32_t freeMemory = CONFIGURE_MAXIMUM_COMMUNICATION_MEMORY - segmentedTask->nextByteToWrite;

    if(segmentedTask == NULL || content == NULL) {
        return RTEMS_EXTENDED_NULL_POINTER;
    }

    /**
     * Covers both cases that either the memory is full aready or
     * that more memory is needed than what's left.
     */
    if(sizeOfContent == 0 || freeMemory < sizeOfContent) {
        return RTEMS_INVALID_SIZE;
    }

    // --- Implementation ---
    for(uint32_t i = 0; i < sizeOfContent; i++) {
        segmentedTask->communicationMemory[i + segmentedTask->nextByteToWrite] = content[i];
    }

    return RTEMS_SUCCESSFUL;
}

rtems_extended_status_code rtems_task_segmented_clear_communication_memory_impl(Segmented_Task_Task* segmentedTask) {
    // --- Argument validation ---
    if(segmentedTask == NULL) {
        return RTEMS_EXTENDED_NULL_POINTER;
    }

    // --- Implementation ---
    for(uint32_t i = 0; i < CONFIGURE_MAXIMUM_COMMUNICATION_MEMORY; i++) {
        segmentedTask->communicationMemory[i] = 0;
    }

    segmentedTask->nextByteToWrite = 0;

    return RTEMS_SUCCESSFUL;
}