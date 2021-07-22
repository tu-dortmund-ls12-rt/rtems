#include "custom_scenario.h"
#include "custom_scenario_impl.h"

/********************************************
 *                                          *
 *       Memory Task - Segment Functions    *
 *                                          *
 ********************************************/
void memoryTaskSegment1(Segmented_Task_Arguments args) {
    rtems_status_code status;

    printf("[Task: Memory Task - Segment: 1]: Started execution.\n");

    CommunicationMemory cm;
    cm.value = 42;
    cm.byte = 2;
    cm.someName[0] = 'H';
    cm.someName[1] = 'e';
    cm.someName[2] = 'l';
    cm.someName[3] = 'l';
    cm.someName[4] = 'o';
    cm.someName[5] = ' ';
    cm.someName[6] = 'W';
    cm.someName[7] = 'o';
    cm.someName[8] = 'r';
    cm.someName[9] = 'l';
    cm.someName[10] = 'd';
    cm.someName[11] = '!';
    cm.someName[12] = '\0';
    cm.functionPtr = structFunction;

    printf("[Task: Memory Task - Segment: 1]: Writing to memory\n");
    status = rtems_task_segmented_write_communication_memory((uint8_t*) &cm, sizeof(cm));
    directive_failed(status, "Failed to write memory\n");

    printf("[Task: Memory Task - Segment: 1]: Created the following communication memory:\n");
    printf("[Task: Memory Task - Segment: 1]: Value: %u\n", cm.value);
    printf("[Task: Memory Task - Segment: 1]: Byte: %u\n", cm.byte);
    printf("[Task: Memory Task - Segment: 1]: SomeName: %s\n", cm.someName);
}

void memoryTaskSegment2(Segmented_Task_Arguments args) {
    rtems_status_code status;

    printf("[Task: Memory Task - Segment: 2]: Started execution.\n");

    CommunicationMemory cm;
    printf("[Task: Memory Task - Segment: 2]: Reading from memory\n");
    status = rtems_task_segmented_read_communication_memory_for((uint8_t*) &cm, sizeof(cm), sizeof(cm));
    directive_failed(status, "Failed to read memory\n");

    printf("[Task: Memory Task - Segment: 2]: Read the following from memory:\n");
    printf("[Task: Memory Task - Segment: 2]: Value: %u\n", cm.value);
    printf("[Task: Memory Task - Segment: 2]: Byte: %u\n", cm.byte);
    printf("[Task: Memory Task - Segment: 2]: SomeName: %s\n", cm.someName);
    printf("[Task: Memory Task - Segment: 2]: Executing function: ");
    cm.functionPtr();
    printf("[Task: Memory Task - Segment: 2]: Finished execution of function.\n");
}

void memoryTaskSegment3(Segmented_Task_Arguments args) {
    rtems_status_code status;

    printf("[Task: Memory Task - Segment: 3]: Started execution.\n");

    CommunicationMemory cm;
    printf("[Task: Memory Task - Segment: 3]: Rereading from memory to check continuity.\n");
    status = rtems_task_segmented_read_communication_memory_for((uint8_t*) &cm, sizeof(cm), sizeof(cm));
    directive_failed(status, "Failed to read memory\n");

    printf("[Task: Memory Task - Segment: 3]: Read the following from memory:\n");
    printf("[Task: Memory Task - Segment: 3]: Value: %u\n", cm.value);
    printf("[Task: Memory Task - Segment: 3]: Byte: %u\n", cm.byte);
    printf("[Task: Memory Task - Segment: 3]: SomeName: %s\n", cm.someName);
    printf("[Task: Memory Task - Segment: 3]: Executing function: ");
    cm.functionPtr();
    printf("[Task: Memory Task - Segment: 3]: Finished execution of function.\n");
}

void structFunction(void) {
  printf("I'm a test function.\n");
}

/********************************************
 *                                          *
 *               Buffer Task                *
 *                                          *
 ********************************************/
void bufferTask(rtems_task_argument args) {
    rtems_status_code status;

    // Most importantly: Directly record start time. Variable declaration/definition can be done afterwards
    rtems_interval startTime;
    status = rtems_clock_get_seconds_since_epoch(&startTime);
    directive_failed(status, "Receiving start time failed.");

    printf("[Task: Buffer Task]: Started execution.\n");

    rtems_interval currentTime;
    uint32_t numberOfSegmentsToResume = bufferTaskReleasedSegementsTotal;
    uint32_t timeToWait = 0;

    uint32_t totalTimeWaited = 0;
    for(uint32_t i = 0; i < numberOfSegmentsToResume; i++) {
        rtems_id taskToResumeId;

        rtems_name taskToResume = bufferTaskReleases[i][0];
        timeToWait = bufferTaskReleases[i][1] - totalTimeWaited;

        status = rtems_clock_get_seconds_since_epoch(&currentTime);
        directive_failed(status, "Receiving current time failed.");
        while(currentTime - startTime < timeToWait) {
            status = rtems_clock_get_seconds_since_epoch(&currentTime);
            directive_failed(status, "Receiving current time failed.");
        }

        status = rtems_task_ident(taskToResume, RTEMS_SEARCH_ALL_NODES, &taskToResumeId);
        directive_failed(status, "Receiving ID of task to release failed.");

        printf("[Task: Buffer Task]: Resuming task %u with id %u.\n", taskToResume, taskToResumeId);

        status = rtems_task_resume_segmented_slfp(taskToResumeId);
        directive_failed(status, "Couldn't resume suspended task.");

        /**
         * Needs to be done this way instead of setting next startTime = currentTime
         * because imagine the case where the resumed task preempts the resuming task.
         * In this case, the difference between a new calculated currentTime and the
         * "old" set startTime would be way greater than it actually is.
         */
        status = rtems_clock_get_seconds_since_epoch(&startTime);
        directive_failed(status, "Receiving new start time within the same segment failed.");

        totalTimeWaited += timeToWait;
    }

    timeToWait = bufferTaskDuration - totalTimeWaited;

    status = rtems_clock_get_seconds_since_epoch(&currentTime);
    directive_failed(status, "Receiving current time failed.");
    while(currentTime - startTime < timeToWait) {
        status = rtems_clock_get_seconds_since_epoch(&currentTime);
        directive_failed(status, "Receiving current time failed.");
    }

    printf("[Task: Buffer Task]: Finished execution.\n");
    rtems_task_exit();
}

/********************************************
 *                                          *
 *                Idle Task                 *
 *                                          *
 ********************************************/
void idleTask(rtems_task_argument args) {
  printf("Non-Idle Task finished. It's missing out rtems_task_exit() directive so the terminal doesn't freeze and the system halts.\n");
}

/********************************************
 *                                          *
 *           Execution Function             *
 *                                          *
 ********************************************/
void custom_scenario_execute(void) {
    printf("Test 2\n");
    rtems_status_code status;

    TEST_BEGIN();

    // Clock init
    rtems_time_of_day time;
    time.ticks = 0;
    time.second = 0;
    time.minute = 0;
    time.hour = 0;
    time.day = 1;
    time.month = 1;
    time.year = 2000;

    status = rtems_clock_set(&time);
    directive_failed(status, "Failed to set the system clock");

    // Task Creation
    status = rtems_task_create_segmented_slfp(memoryTaskName, taskStackSize, taskModes, taskAttributes,
                                              memoryTaskSegments, memoryTaskSegmentFunctions,
                                              memoryTaskPriorities, &memoryTaskId);
    directive_failed(status, "Creation of memory task failed.");

    status = rtems_task_create(bufferTaskName, bufferTaskPriority, taskStackSize, taskModes, taskAttributes, &bufferTaskId);
    directive_failed(status, "Creation of buffer task failed.");

    status = rtems_task_create(idleTaskName, idleTaskPriority, taskStackSize, taskModes, taskAttributes,
                               &idleTaskId);
    directive_failed(status, "Creation of idle task failed.");
    
    // Task Starting
    status = rtems_task_start_segmented_slfp(memoryTaskId, (rtems_task_argument) NULL);
    directive_failed(status, "Start of memory task failed.");

    status = rtems_task_start(bufferTaskId, bufferTask, (rtems_task_argument) NULL);
    directive_failed(status, "Start of buffer task failed.");

    status = rtems_task_start(idleTaskId, idleTask, (rtems_task_argument) NULL);
    directive_failed(status, "Start of idle Task failed.");

    puts("Exiting initialization Task");
    rtems_task_exit();
}