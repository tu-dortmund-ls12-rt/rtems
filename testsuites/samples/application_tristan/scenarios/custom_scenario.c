#include "custom_scenario.h"
#include "custom_scenario_impl.h"

/********************************************
 *                                          *
 *       SLFP Task - Segment Functions    *
 *                                          *
 ********************************************/
void slfpTaskSegment1(Segmented_Task_Arguments args) {
    rtems_status_code status;

    printf("[Task: SLFP Task - Segment: 1]: Started execution.\n");
    printf("[Task: SLFP Task - Segment: 1]: Spinning for two seconds.\n");
    rtems_test_spin_for_ticks(2);
    printf("[Task: SLFP Task - Segment: 1]: Finished execution.\n");
}

void slfpTaskSegment2(Segmented_Task_Arguments args) {
    rtems_status_code status;

    printf("[Task: SLFP Task - Segment: 2]: Started execution.\n");
    printf("[Task: SLFP Task - Segment: 2]: Spinning for two seconds.\n");
    rtems_test_spin_for_ticks(2);
    printf("[Task: SLFP Task - Segment: 2]: Finished execution.\n");
}

void slfpTaskSegment3(Segmented_Task_Arguments args) {
    rtems_status_code status;

    printf("[Task: SLFP Task - Segment: 3]: Started execution.\n");
    printf("[Task: SLFP Task - Segment: 3]: Spinning for two seconds.\n");
    rtems_test_spin_for_ticks(2);
    printf("[Task: SLFP Task - Segment: 3]: Finished execution.\n");
}

/********************************************
 *                                          *
 *                Idle Task                 *
 *                                          *
 ********************************************/
void idleTask(rtems_task_argument args) {
    rtems_status_code status;
    rtems_id taskToResume;
    bool resume = true;

    printf("[Task: Idle Task]: Started execution.\n");
    printf("[Task: Idle Task]: The idle task will run forever, to prevent system stallage.\n");

    status = rtems_task_ident(slfpTaskName, RTEMS_SEARCH_ALL_NODES, &taskToResume);
    if(!rtems_is_status_successful(status)) {
        directive_failed(status, "Receiving the id of slfp task failed.");
        printf("[Task: Idle Task]: The idle task will continue but not resume anything.\n");
        resume = false;
    }

    while(true) {
        printf("[Task: Idle Task]: Spinning for one second.\n");
        rtems_test_spin_for_ticks(1);

        if(resume) {
            status = rtems_task_resume_segmented_slfp(taskToResume);
            directive_failed(status, "Resuming slfp task failed");
        }
    }
}

/********************************************
 *                                          *
 *           Execution Function             *
 *                                          *
 ********************************************/
void custom_scenario_execute(void) {
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
    status = rtems_task_create_segmented_slfp(slfpTaskName, taskStackSize, taskModes, taskAttributes, slfpTaskPeriod,
                                              slfpTaskSegments, slfpTaskSegmentFunctions,
                                              slfpTaskPriorities, &slfpTaskId, &slfpTaskPeriodId);
    directive_failed(status, "Creation of memory task failed.");

    status = rtems_task_create(idleTaskName, idleTaskPriority, taskStackSize, taskModes, taskAttributes,
                               &idleTaskId);
    directive_failed(status, "Creation of idle task failed.");
    
    // Task Starting
    status = rtems_task_start_segmented_slfp(slfpTaskId, (rtems_task_argument) NULL);
    directive_failed(status, "Start of memory task failed.");

    status = rtems_task_start(idleTaskId, idleTask, (rtems_task_argument) NULL);
    directive_failed(status, "Start of idle Task failed.");

    puts("Exiting initialization Task");
    rtems_task_exit();
}