#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "system.h"

const char rtems_test_name[] = "APPLICATION TRISTAN";
rtems_name segmentedTaskName = rtems_build_name('S', 'e', 'g', ' ');
rtems_name bufferTaskName = rtems_build_name('B', 'u', 'f', ' ');
rtems_name idleTaskName = rtems_build_name('I', 'd', 'l', 'e');

//----------------------

/**
 * Test scenario:
 * - Three segmented tasks scheduled with slfp + one idle task
 * 
 * - First Task:
 *    - 3 Segments
 *    - First Segment:
 *      - Priority: 1
 *      - Duration: 3s
 *      - Releases: Nothing
 *    - Second Segment:
 *      - Priority: 1
 *      - Duration: 5
 *      - Releases: Task 2 at 2s
 *    - Third Segment:
 *      - Priority: 5
 *      - Duration: 2
 *      - Releases: Nothing
 * 
 * - Second Task:
 *    - 2 Segments
 *    - First Segment:
 *      - Priority: 2
 *      - Duration: 1
 *      - Releases: Nothing
 *    - Second Segment:
 *      - Priority: 6
 *      - Duration: 5
 *      - Releases: Task 3 at 4s and Task 3 at 5s
 * 
 * - Third Task:
 *    - 3 Segments
 *    - First Segment:
 *      - Priority: 3
 *      - Duration: 4
 *      - Releases: Task 1 at 2s
 *    - Second Segment:
 *      - Priority: 4
 *      - Duration: 3
 *      - Releases: Task 1 at 1s
 *    - Third Segment:
 *      - Priority: 7
 *      - Duration: 2
 *      - Releases: Nothing
 */

// General
const size_t taskStackSize = RTEMS_MINIMUM_STACK_SIZE;
const rtems_mode taskModes = RTEMS_DEFAULT_MODES;
const rtems_attribute taskAttributes = RTEMS_DEFAULT_ATTRIBUTES;

const rtems_name task1Name = rtems_build_name('T', '1', ' ', ' ');
const rtems_name task2Name = rtems_build_name('T', '2', ' ', ' ');
const rtems_name task3Name = rtems_build_name('T', '3', ' ', ' ');

uint32_t taskArguments[] = {10, 20, 30};

// Task 1
void task1segment1(Segmented_Task_Arguments args);
void task1segment2(Segmented_Task_Arguments args);
void task1segment3(Segmented_Task_Arguments args);

rtems_id task1Id;
const uint32_t task1Segments = 3;
rtems_task_priority task1Priorities[3] = {1, 1, 5};
const uint32_t task1durations[3] = {3, 5, 2};
const uint32_t task1ReleasedSegementsTotal = 1;
const uint32_t task1ReleasedSegments[3] = {0, 1, 0};
/**
 * Syntax:
 * {Task which is to resume, When to resume it with respect to the resuming segments start time}
 */
const uint32_t task1Releases[1][2] = {{task2Name, 2}};
const void (*task1SegmentFunctions[]) (Segmented_Task_Arguments args) = {task1segment1, task1segment2, task1segment3};

void task1segment1(Segmented_Task_Arguments args) {
  rtems_status_code status;

  // Most importantly: Directly record start time. Variable declaration/definition can be done afterwards
  rtems_interval startTime;
  status = rtems_clock_get_seconds_since_epoch(&startTime);
  directive_failed(status, "Receiving start time failed.");

  printf("[Task: 1 - Segegment: 1]: Started execution.\n");

  rtems_interval currentTime;
  uint32_t currentSegment = 1; // Current segment number, not index.
  uint32_t numberOfSegementsToResume = task1ReleasedSegments[currentSegment - 1];
  uint32_t offset = 0;
  uint32_t timeToWait = 0;

  // Argument tests
  uint32_t arg = ((uint32_t*) args.arguments)[currentSegment - 1];
  printf("[Task: 1 - Segegment: 1]: ------------------\n");
  printf("[Task: 1 - Segegment: 1]: Testing arguments.\n");
  printf("[Task: 1 - Segegment: 1]: Passed current segment: %u - Should be: %u\n", args.currentSegment, currentSegment - 1);
  printf("[Task: 1 - Segegment: 1]: Passed argument: %u\n", arg);
  printf("[Task: 1 - Segegment: 1]: ------------------\n");

  /**
   * Step 1: Wieviele Segmente müssen in diesem Segment released werden?
   * Step 2: Für jedes Segment das released werden muss:
   * Step 2.1: Errechne die Zeit bis zum release
   * Step 2.2: Warte die Zeit bis zum release.
   * Step 2.3: Release.
   * Step 3: Warte die restliche Zeit vom Segment
   * Step 4: Exit
   */ 

  // Determine resume information index offset
  for(uint32_t i = 0; i < currentSegment - 1; i++) {
    offset += task1ReleasedSegments[i];
  }

  // Step 2
  uint32_t totalTimeWaited = 0;
  for(uint32_t i = 0; i < numberOfSegementsToResume; i++) {
    rtems_id taskToResumeId;

    // Step 2.1
    rtems_name taskToResume = task1Releases[offset + i][0];
    timeToWait = task1Releases[offset + i][1] - totalTimeWaited;

    // Step 2.2
    status = rtems_clock_get_seconds_since_epoch(&currentTime);
    directive_failed(status, "Receiving current time failed.");
    while(currentTime - startTime < timeToWait) {
      status = rtems_clock_get_seconds_since_epoch(&currentTime);
      directive_failed(status, "Receiving current time failed.");
    }

    // Step 2.3
    status = rtems_task_ident(taskToResume, RTEMS_SEARCH_ALL_NODES, &taskToResumeId);
    directive_failed(status, "Receiving ID of task to release failed.");

    printf("[Task: 1 - Segegment: 1]: Resuming task %u with id %u.\n", taskToResume, taskToResumeId);

    status = rtems_task_resume_segmented_slfp(taskToResumeId);
    directive_failed(status, "Couldn't resume suspended task");

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

  // Step 3
  timeToWait = task1durations[currentSegment - 1] - totalTimeWaited;

  status = rtems_clock_get_seconds_since_epoch(&currentTime);
  directive_failed(status, "Receiving current time failed.");
  while(currentTime - startTime < timeToWait) {
    status = rtems_clock_get_seconds_since_epoch(&currentTime);
    directive_failed(status, "Receiving current time failed.");
  }

  printf("[Task: 1 - Segegment: 1]: Finished execution.\n");
}

void task1segment2(Segmented_Task_Arguments args) {
  rtems_status_code status;

  // Most importantly: Directly record start time. Variable declaration/definition can be done afterwards
  rtems_interval startTime;
  status = rtems_clock_get_seconds_since_epoch(&startTime);
  directive_failed(status, "Receiving start time failed.");

  printf("[Task: 1 - Segegment: 2]: Started execution.\n");

  rtems_interval currentTime;
  uint32_t currentSegment = 2; // Current segment number, not index.
  uint32_t numberOfSegementsToResume = task1ReleasedSegments[currentSegment - 1];
  uint32_t offset = 0;
  uint32_t timeToWait = 0;

  // Argument tests
  uint32_t arg = ((uint32_t*) args.arguments)[currentSegment - 1];
  printf("[Task: 1 - Segegment: 2]: ------------------\n");
  printf("[Task: 1 - Segegment: 2]: Testing arguments.\n");
  printf("[Task: 1 - Segegment: 2]: Passed current segment: %u - Should be: %u\n", args.currentSegment, currentSegment - 1);
  printf("[Task: 1 - Segegment: 2]: Passed argument: %u\n", arg);
  printf("[Task: 1 - Segegment: 2]: ------------------\n");

  /**
   * Step 1: Wieviele Segmente müssen in diesem Segment released werden?
   * Step 2: Für jedes Segment das released werden muss:
   * Step 2.1: Errechne die Zeit bis zum release
   * Step 2.2: Warte die Zeit bis zum release.
   * Step 2.3: Release.
   * Step 3: Warte die restliche Zeit vom Segment
   * Step 4: Exit
   */ 

  // Determine resume information index offset
  for(uint32_t i = 0; i < currentSegment - 1; i++) {
    offset += task1ReleasedSegments[i];
  }

  // Step 2
  uint32_t totalTimeWaited = 0;
  for(uint32_t i = 0; i < numberOfSegementsToResume; i++) {
    rtems_id taskToResumeId;

    // Step 2.1
    rtems_name taskToResume = task1Releases[offset + i][0];
    timeToWait = task1Releases[offset + i][1] - totalTimeWaited;

    // Step 2.2
    status = rtems_clock_get_seconds_since_epoch(&currentTime);
    directive_failed(status, "Receiving current time failed.");
    while(currentTime - startTime < timeToWait) {
      status = rtems_clock_get_seconds_since_epoch(&currentTime);
      directive_failed(status, "Receiving current time failed.");
    }

    // Step 2.3
    status = rtems_task_ident(taskToResume, RTEMS_SEARCH_ALL_NODES, &taskToResumeId);
    directive_failed(status, "Receiving ID of task to release failed.");

    printf("[Task: 1 - Segegment: 2]: Resuming task %u with id %u.\n", taskToResume, taskToResumeId);

    status = rtems_task_resume_segmented_slfp(taskToResumeId);
    directive_failed(status, "Couldn't resume suspended task");

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

  // Step 3
  timeToWait = task1durations[currentSegment - 1] - totalTimeWaited;

  status = rtems_clock_get_seconds_since_epoch(&currentTime);
  directive_failed(status, "Receiving current time failed.");
  while(currentTime - startTime < timeToWait) {
    status = rtems_clock_get_seconds_since_epoch(&currentTime);
    directive_failed(status, "Receiving current time failed.");
  }

  printf("[Task: 1 - Segegment: 2]: Finished execution.\n");
}

void task1segment3(Segmented_Task_Arguments args) {
  rtems_status_code status;

  // Most importantly: Directly record start time. Variable declaration/definition can be done afterwards
  rtems_interval startTime;
  status = rtems_clock_get_seconds_since_epoch(&startTime);
  directive_failed(status, "Receiving start time failed.");

  printf("[Task: 1 - Segegment: 3]: Started execution.\n");

  rtems_interval currentTime;
  uint32_t currentSegment = 3; // Current segment number, not index.
  uint32_t numberOfSegementsToResume = task1ReleasedSegments[currentSegment - 1];
  uint32_t offset = 0;
  uint32_t timeToWait = 0;

  // Argument tests
  uint32_t arg = ((uint32_t*) args.arguments)[currentSegment - 1];
  printf("[Task: 1 - Segegment: 1]: ------------------\n");
  printf("[Task: 1 - Segegment: 1]: Testing arguments.\n");
  printf("[Task: 1 - Segegment: 1]: Passed current segment: %u - Should be: %u\n", args.currentSegment, currentSegment - 1);
  printf("[Task: 1 - Segegment: 1]: Passed argument: %u\n", arg);
  printf("[Task: 1 - Segegment: 1]: ------------------\n");

  /**
   * Step 1: Wieviele Segmente müssen in diesem Segment released werden?
   * Step 2: Für jedes Segment das released werden muss:
   * Step 2.1: Errechne die Zeit bis zum release
   * Step 2.2: Warte die Zeit bis zum release.
   * Step 2.3: Release.
   * Step 3: Warte die restliche Zeit vom Segment
   * Step 4: Exit
   */ 

  // Determine resume information index offset
  for(uint32_t i = 0; i < currentSegment - 1; i++) {
    offset += task1ReleasedSegments[i];
  }

  // Step 2
  uint32_t totalTimeWaited = 0;
  for(uint32_t i = 0; i < numberOfSegementsToResume; i++) {
    rtems_id taskToResumeId;

    // Step 2.1
    rtems_name taskToResume = task1Releases[offset + i][0];
    timeToWait = task1Releases[offset + i][1] - totalTimeWaited;

    // Step 2.2
    status = rtems_clock_get_seconds_since_epoch(&currentTime);
    directive_failed(status, "Receiving current time failed.");
    while(currentTime - startTime < timeToWait) {
      status = rtems_clock_get_seconds_since_epoch(&currentTime);
      directive_failed(status, "Receiving current time failed.");
    }

    // Step 2.3
    status = rtems_task_ident(taskToResume, RTEMS_SEARCH_ALL_NODES, &taskToResumeId);
    directive_failed(status, "Receiving ID of task to release failed.");

    printf("[Task: 1 - Segegment: 3]: Resuming task %u with id %u.\n", taskToResume, taskToResumeId);

    status = rtems_task_resume_segmented_slfp(taskToResumeId);
    directive_failed(status, "Couldn't resume suspended task");

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

  // Step 3
  timeToWait = task1durations[currentSegment - 1] - totalTimeWaited;

  status = rtems_clock_get_seconds_since_epoch(&currentTime);
  directive_failed(status, "Receiving current time failed.");
  while(currentTime - startTime < timeToWait) {
    status = rtems_clock_get_seconds_since_epoch(&currentTime);
    directive_failed(status, "Receiving current time failed.");
  }

  printf("[Task: 1 - Segegment: 3]: Finished execution.\n");
}

// Task 2
void task2segment1(Segmented_Task_Arguments args);
void task2segment2(Segmented_Task_Arguments args);

rtems_id task2Id;
const uint32_t task2Segments = 2;
rtems_task_priority task2Priorities[2] = {2, 6};
const uint32_t task2durations[2] = {1, 5};
const uint32_t task2ReleasedSegementsTotal = 2;
const uint32_t task2ReleasedSegments[2] = {0, 2};
const uint32_t task2Releases[2][2] = {{task3Name, 4}, {task3Name, 5}}; // {Task which is to release, when to release}
const void (*task2SegmentFunctions[]) (Segmented_Task_Arguments args) = {task2segment1, task2segment2};

void task2segment1(Segmented_Task_Arguments args) {
  rtems_status_code status;

  // Most importantly: Directly record start time. Variable declaration/definition can be done afterwards
  rtems_interval startTime;
  status = rtems_clock_get_seconds_since_epoch(&startTime);
  directive_failed(status, "Receiving start time failed.");

  printf("[Task: 2 - Segegment: 1]: Started execution.\n");

  rtems_interval currentTime;
  uint32_t currentSegment = 1; // Current segment number, not index.
  uint32_t numberOfSegementsToResume = task2ReleasedSegments[currentSegment - 1];
  uint32_t offset = 0;
  uint32_t timeToWait = 0;

  // Argument tests
  uint32_t arg = ((uint32_t*) args.arguments)[currentSegment - 1];
  printf("[Task: 2 - Segegment: 1]: ------------------\n");
  printf("[Task: 2 - Segegment: 1]: Testing arguments.\n");
  printf("[Task: 2 - Segegment: 1]: Passed current segment: %u - Should be: %u\n", args.currentSegment, currentSegment - 1);
  printf("[Task: 2 - Segegment: 1]: Passed argument: %u\n", arg);
  printf("[Task: 2 - Segegment: 1]: ------------------\n");

  /**
   * Step 1: Wieviele Segmente müssen in diesem Segment released werden?
   * Step 2: Für jedes Segment das released werden muss:
   * Step 2.1: Errechne die Zeit bis zum release
   * Step 2.2: Warte die Zeit bis zum release.
   * Step 2.3: Release.
   * Step 3: Warte die restliche Zeit vom Segment
   * Step 4: Exit
   */ 

  // Determine resume information index offset
  for(uint32_t i = 0; i < currentSegment - 1; i++) {
    offset += task2ReleasedSegments[i];
  }

  // Step 2
  uint32_t totalTimeWaited = 0;
  for(uint32_t i = 0; i < numberOfSegementsToResume; i++) {
    rtems_id taskToResumeId;

    // Step 2.1
    rtems_name taskToResume = task2Releases[offset + i][0];
    timeToWait = task2Releases[offset + i][1] - totalTimeWaited;

    // Step 2.2
    status = rtems_clock_get_seconds_since_epoch(&currentTime);
    directive_failed(status, "Receiving current time failed.");
    while(currentTime - startTime < timeToWait) {
      status = rtems_clock_get_seconds_since_epoch(&currentTime);
      directive_failed(status, "Receiving current time failed.");
    }

    // Step 2.3
    status = rtems_task_ident(taskToResume, RTEMS_SEARCH_ALL_NODES, &taskToResumeId);
    directive_failed(status, "Receiving ID of task to release failed.");

    printf("[Task: 2 - Segegment: 1]: Resuming task %u with id %u.\n", taskToResume, taskToResumeId);

    status = rtems_task_resume_segmented_slfp(taskToResumeId);
    directive_failed(status, "Couldn't resume suspended task");

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

  // Step 3
  timeToWait = task2durations[currentSegment - 1] - totalTimeWaited;

  status = rtems_clock_get_seconds_since_epoch(&currentTime);
  directive_failed(status, "Receiving current time failed.");
  while(currentTime - startTime < timeToWait) {
    status = rtems_clock_get_seconds_since_epoch(&currentTime);
    directive_failed(status, "Receiving current time failed.");
  }

  printf("[Task: 2 - Segegment: 1]: Finished execution.\n");
}

void task2segment2(Segmented_Task_Arguments args) {
  rtems_status_code status;

  // Most importantly: Directly record start time. Variable declaration/definition can be done afterwards
  rtems_interval startTime;
  status = rtems_clock_get_seconds_since_epoch(&startTime);
  directive_failed(status, "Receiving start time failed.");

  printf("[Task: 2 - Segegment: 2]: Started execution.\n");

  rtems_interval currentTime;
  uint32_t currentSegment = 2; // Current segment number, not index.
  uint32_t numberOfSegementsToResume = task2ReleasedSegments[currentSegment - 1];
  uint32_t offset = 0;
  uint32_t timeToWait = 0;

  // Argument tests
  uint32_t arg = ((uint32_t*) args.arguments)[currentSegment - 1];
  printf("[Task: 2 - Segegment: 2]: ------------------\n");
  printf("[Task: 2 - Segegment: 2]: Testing arguments.\n");
  printf("[Task: 2 - Segegment: 2]: Passed current segment: %u - Should be: %u\n", args.currentSegment, currentSegment - 1);
  printf("[Task: 2 - Segegment: 2]: Passed argument: %u\n", arg);
  printf("[Task: 2 - Segegment: 2]: ------------------\n");

  /**
   * Step 1: Wieviele Segmente müssen in diesem Segment released werden?
   * Step 2: Für jedes Segment das released werden muss:
   * Step 2.1: Errechne die Zeit bis zum release
   * Step 2.2: Warte die Zeit bis zum release.
   * Step 2.3: Release.
   * Step 3: Warte die restliche Zeit vom Segment
   * Step 4: Exit
   */ 

  // Determine resume information index offset
  for(uint32_t i = 0; i < currentSegment - 1; i++) {
    offset += task2ReleasedSegments[i];
  }

  // Step 2
  uint32_t totalTimeWaited = 0;
  for(uint32_t i = 0; i < numberOfSegementsToResume; i++) {
    rtems_id taskToResumeId;

    // Step 2.1
    rtems_name taskToResume = task2Releases[offset + i][0];
    timeToWait = task2Releases[offset + i][1] - totalTimeWaited;

    // Step 2.2
    status = rtems_clock_get_seconds_since_epoch(&currentTime);
    directive_failed(status, "Receiving current time failed.");
    while(currentTime - startTime < timeToWait) {
      status = rtems_clock_get_seconds_since_epoch(&currentTime);
      directive_failed(status, "Receiving current time failed.");
    }

    // Step 2.3
    status = rtems_task_ident(taskToResume, RTEMS_SEARCH_ALL_NODES, &taskToResumeId);
    directive_failed(status, "Receiving ID of task to release failed.");

    printf("[Task: 2 - Segegment: 2]: Resuming task %u with id %u.\n", taskToResume, taskToResumeId);

    status = rtems_task_resume_segmented_slfp(taskToResumeId);
    directive_failed(status, "Couldn't resume suspended task");

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

  // Step 3
  timeToWait = task2durations[currentSegment - 1] - totalTimeWaited;

  status = rtems_clock_get_seconds_since_epoch(&currentTime);
  directive_failed(status, "Receiving current time failed.");
  while(currentTime - startTime < timeToWait) {
    status = rtems_clock_get_seconds_since_epoch(&currentTime);
    directive_failed(status, "Receiving current time failed.");
  }

  printf("[Task: 2 - Segegment: 2]: Finished execution.\n");
}

// Task 3
void task3segment1(Segmented_Task_Arguments args);
void task3segment2(Segmented_Task_Arguments args);
void task3segment3(Segmented_Task_Arguments args);

rtems_id task3Id;
const uint32_t task3Segments = 3;
rtems_task_priority task3Priorities[3] = {3, 4, 7};
const uint32_t task3durations[3] = {4, 3, 2};
const uint32_t task3ReleasedSegementsTotal = 2;
const uint32_t task3ReleasedSegments[3] = {1, 1, 0};
const uint32_t task3Releases[2][2] = {{task1Name, 2}, {task1Name, 1}}; // {Task which is to release, when to release}
const void (*task3SegmentFunctions[]) (Segmented_Task_Arguments args) = {task3segment1, task3segment2, task3segment3};

void task3segment1(Segmented_Task_Arguments args) {
  rtems_status_code status;

  // Most importantly: Directly record start time. Variable declaration/definition can be done afterwards
  rtems_interval startTime;
  status = rtems_clock_get_seconds_since_epoch(&startTime);
  directive_failed(status, "Receiving start time failed.");

  printf("[Task: 3 - Segegment: 1]: Started execution.\n");

  rtems_interval currentTime;
  uint32_t currentSegment = 1; // Current segment number, not index.
  uint32_t numberOfSegementsToResume = task3ReleasedSegments[currentSegment - 1];
  uint32_t offset = 0;
  uint32_t timeToWait = 0;

  // Argument tests
  uint32_t arg = ((uint32_t*) args.arguments)[currentSegment - 1];
  printf("[Task: 3 - Segegment: 1]: ------------------\n");
  printf("[Task: 3 - Segegment: 1]: Testing arguments.\n");
  printf("[Task: 3 - Segegment: 1]: Passed current segment: %u - Should be: %u\n", args.currentSegment, currentSegment - 1);
  printf("[Task: 3 - Segegment: 1]: Passed argument: %u\n", arg);
  printf("[Task: 3 - Segegment: 1]: ------------------\n");

  /**
   * Step 1: Wieviele Segmente müssen in diesem Segment released werden?
   * Step 2: Für jedes Segment das released werden muss:
   * Step 2.1: Errechne die Zeit bis zum release
   * Step 2.2: Warte die Zeit bis zum release.
   * Step 2.3: Release.
   * Step 3: Warte die restliche Zeit vom Segment
   * Step 4: Exit
   */ 

  // Determine resume information index offset
  for(uint32_t i = 0; i < currentSegment - 1; i++) {
    offset += task3ReleasedSegments[i];
  }

  // Step 2
  uint32_t totalTimeWaited = 0;
  for(uint32_t i = 0; i < numberOfSegementsToResume; i++) {
    rtems_id taskToResumeId;

    // Step 2.1
    rtems_name taskToResume = task3Releases[offset + i][0];
    timeToWait = task3Releases[offset + i][1] - totalTimeWaited;

    // Step 2.2
    status = rtems_clock_get_seconds_since_epoch(&currentTime);
    directive_failed(status, "Receiving current time failed.");
    while(currentTime - startTime < timeToWait) {
      status = rtems_clock_get_seconds_since_epoch(&currentTime);
      directive_failed(status, "Receiving current time failed.");
    }

    // Step 2.3
    status = rtems_task_ident(taskToResume, RTEMS_SEARCH_ALL_NODES, &taskToResumeId);
    directive_failed(status, "Receiving ID of task to release failed.");

    printf("[Task: 3 - Segegment: 1]: Resuming task %u with id %u.\n", taskToResume, taskToResumeId);

    status = rtems_task_resume_segmented_slfp(taskToResumeId);
    directive_failed(status, "Couldn't resume suspended task");

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

  // Step 3
  timeToWait = task3durations[currentSegment - 1] - totalTimeWaited;

  status = rtems_clock_get_seconds_since_epoch(&currentTime);
  directive_failed(status, "Receiving current time failed.");
  while(currentTime - startTime < timeToWait) {
    status = rtems_clock_get_seconds_since_epoch(&currentTime);
    directive_failed(status, "Receiving current time failed.");
  }

  printf("[Task: 3 - Segegment: 1]: Finished execution.\n");
}

void task3segment2(Segmented_Task_Arguments args) {
  rtems_status_code status;

  // Most importantly: Directly record start time. Variable declaration/definition can be done afterwards
  rtems_interval startTime;
  status = rtems_clock_get_seconds_since_epoch(&startTime);
  directive_failed(status, "Receiving start time failed.");

  printf("[Task: 3 - Segegment: 2]: Started execution.\n");

  rtems_interval currentTime;
  uint32_t currentSegment = 2; // Current segment number, not index.
  uint32_t numberOfSegementsToResume = task3ReleasedSegments[currentSegment - 1];
  uint32_t offset = 0;
  uint32_t timeToWait = 0;

  // Argument tests
  uint32_t arg = ((uint32_t*) args.arguments)[currentSegment - 1];
  printf("[Task: 3 - Segegment: 2]: ------------------\n");
  printf("[Task: 3 - Segegment: 2]: Testing arguments.\n");
  printf("[Task: 3 - Segegment: 2]: Passed current segment: %u - Should be: %u\n", args.currentSegment, currentSegment - 1);
  printf("[Task: 3 - Segegment: 2]: Passed argument: %u\n", arg);
  printf("[Task: 3 - Segegment: 2]: ------------------\n");

  /**
   * Step 1: Wieviele Segmente müssen in diesem Segment released werden?
   * Step 2: Für jedes Segment das released werden muss:
   * Step 2.1: Errechne die Zeit bis zum release
   * Step 2.2: Warte die Zeit bis zum release.
   * Step 2.3: Release.
   * Step 3: Warte die restliche Zeit vom Segment
   * Step 4: Exit
   */ 

  // Determine resume information index offset
  for(uint32_t i = 0; i < currentSegment - 1; i++) {
    offset += task3ReleasedSegments[i];
  }

  // Step 2
  uint32_t totalTimeWaited = 0;
  for(uint32_t i = 0; i < numberOfSegementsToResume; i++) {
    rtems_id taskToResumeId;

    // Step 2.1
    rtems_name taskToResume = task3Releases[offset + i][0];
    timeToWait = task3Releases[offset + i][1] - totalTimeWaited;

    // Step 2.2
    status = rtems_clock_get_seconds_since_epoch(&currentTime);
    directive_failed(status, "Receiving current time failed.");
    while(currentTime - startTime < timeToWait) {
      status = rtems_clock_get_seconds_since_epoch(&currentTime);
      directive_failed(status, "Receiving current time failed.");
    }

    // Step 2.3
    status = rtems_task_ident(taskToResume, RTEMS_SEARCH_ALL_NODES, &taskToResumeId);
    directive_failed(status, "Receiving ID of task to release failed.");

    printf("[Task: 3 - Segegment: 2]: Resuming task %u with id %u.\n", taskToResume, taskToResumeId);

    status = rtems_task_resume_segmented_slfp(taskToResumeId);
    directive_failed(status, "Couldn't resume suspended task");

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

  // Step 3
  timeToWait = task3durations[currentSegment - 1] - totalTimeWaited;

  status = rtems_clock_get_seconds_since_epoch(&currentTime);
  directive_failed(status, "Receiving current time failed.");
  while(currentTime - startTime < timeToWait) {
    status = rtems_clock_get_seconds_since_epoch(&currentTime);
    directive_failed(status, "Receiving current time failed.");
  }

  printf("[Task: 3 - Segegment: 2]: Finished execution.\n");
}

void task3segment3(Segmented_Task_Arguments args) {
  rtems_status_code status;

  // Most importantly: Directly record start time. Variable declaration/definition can be done afterwards
  rtems_interval startTime;
  status = rtems_clock_get_seconds_since_epoch(&startTime);
  directive_failed(status, "Receiving start time failed.");

  printf("[Task: 3 - Segegment: 3]: Started execution.\n");

  rtems_interval currentTime;
  uint32_t currentSegment = 3; // Current segment number, not index.
  uint32_t numberOfSegementsToResume = task3ReleasedSegments[currentSegment - 1];
  uint32_t offset = 0;
  uint32_t timeToWait = 0;

  // Argument tests
  uint32_t arg = ((uint32_t*) args.arguments)[currentSegment - 1];
  printf("[Task: 3 - Segegment: 3]: ------------------\n");
  printf("[Task: 3 - Segegment: 3]: Testing arguments.\n");
  printf("[Task: 3 - Segegment: 3]: Passed current segment: %u - Should be: %u\n", args.currentSegment, currentSegment - 1);
  printf("[Task: 3 - Segegment: 3]: Passed argument: %u\n", arg);
  printf("[Task: 3 - Segegment: 3]: ------------------\n");

  /**
   * Step 1: Wieviele Segmente müssen in diesem Segment released werden?
   * Step 2: Für jedes Segment das released werden muss:
   * Step 2.1: Errechne die Zeit bis zum release
   * Step 2.2: Warte die Zeit bis zum release.
   * Step 2.3: Release.
   * Step 3: Warte die restliche Zeit vom Segment
   * Step 4: Exit
   */ 

  // Determine resume information index offset
  for(uint32_t i = 0; i < currentSegment - 1; i++) {
    offset += task3ReleasedSegments[i];
  }

  // Step 2
  uint32_t totalTimeWaited = 0;
  for(uint32_t i = 0; i < numberOfSegementsToResume; i++) {
    rtems_id taskToResumeId;

    // Step 2.1
    rtems_name taskToResume = task3Releases[offset + i][0];
    timeToWait = task3Releases[offset + i][1] - totalTimeWaited;

    // Step 2.2
    status = rtems_clock_get_seconds_since_epoch(&currentTime);
    directive_failed(status, "Receiving current time failed.");
    while(currentTime - startTime < timeToWait) {
      status = rtems_clock_get_seconds_since_epoch(&currentTime);
      directive_failed(status, "Receiving current time failed.");
    }

    // Step 2.3
    status = rtems_task_ident(taskToResume, RTEMS_SEARCH_ALL_NODES, &taskToResumeId);
    directive_failed(status, "Receiving ID of task to release failed.");

    printf("[Task: 3 - Segegment: 3]: Resuming task %u with id %u.\n", taskToResume, taskToResumeId);

    status = rtems_task_resume_segmented_slfp(taskToResumeId);
    directive_failed(status, "Couldn't resume suspended task");

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

  // Step 3
  timeToWait = task3durations[currentSegment - 1] - totalTimeWaited;

  status = rtems_clock_get_seconds_since_epoch(&currentTime);
  directive_failed(status, "Receiving current time failed.");
  while(currentTime - startTime < timeToWait) {
    status = rtems_clock_get_seconds_since_epoch(&currentTime);
    directive_failed(status, "Receiving current time failed.");
  }

  printf("[Task: 3 - Segegment: 3]: Finished execution.\n");
}

//----------------------

static rtems_task buffer(rtems_task_argument argument) {
  uint32_t secondsToWait = 3;

  rtems_status_code status;

  printf("Buffer is beeing executed.\n");

  for(uint32_t i = 0; i < 2; i++) {
    rtems_interval waitStart;
    rtems_interval currentTime;

    status = rtems_clock_get_seconds_since_epoch(&waitStart);
    directive_failed(status, "Receiving start time failed.");

    currentTime = waitStart;

    while(currentTime - waitStart < secondsToWait) {
      status = rtems_clock_get_seconds_since_epoch(&currentTime);
      directive_failed(status, "Receiving current time failed.");
    }

    // Unsuspend the segmented task (is there to simulate the external event)
    rtems_id taskId;
    status = rtems_task_ident(segmentedTaskName, RTEMS_SEARCH_ALL_NODES, &taskId);
    directive_failed(status, "Couldn't receive task ID.");
    
    printf("Resuming suspended task\n");

    status = rtems_task_resume_segmented_slfp(taskId);
    directive_failed(status, "Couldn't resume suspended task");
  }

  printf("Simulating active execution in the buffer task for 10 seconds...");
  secondsToWait = 10;
  rtems_interval simStart;
  rtems_interval lastDot;
  rtems_interval simCurrent;

  status = rtems_clock_get_seconds_since_epoch(&simStart);
  directive_failed(status, "Receiving sim start time failed");

  lastDot = simStart;
  simCurrent = simStart;

  while(simCurrent - simStart < secondsToWait) {
    if(simCurrent - lastDot >= 1) {
      printf(".");
      lastDot = simCurrent;
    }

    status = rtems_clock_get_seconds_since_epoch(&simCurrent);
    directive_failed(status, "Receiving current time failed.");
  }

  printf("\n");

  printf("Finished simulating active execution.\n");
  rtems_task_exit();
}

static rtems_task idle(rtems_task_argument argument) {
  printf("Non-Idle Task finished. It's missing out rtems_task_exit() directive so the terminal doesn't freeze and the system halts.\n");
}

rtems_task Init(
	rtems_task_argument argument
)
{
  rtems_status_code status;
  rtems_id idleTaskId;

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
  status = rtems_task_create_segmented_slfp(task1Name, taskStackSize, taskModes, taskAttributes,
                                            task1Segments, task1SegmentFunctions, task1Priorities,
                                            &task1Id);
  directive_failed(status, "Creation of segmented task 1 failed.");

  status = rtems_task_create_segmented_slfp(task2Name, taskStackSize, taskModes, taskAttributes,
                                            task2Segments, task2SegmentFunctions, task2Priorities,
                                            &task2Id);
  directive_failed(status, "Creation of segmented task 2 failed.");

  status = rtems_task_create_segmented_slfp(task3Name, taskStackSize, taskModes, taskAttributes,
                                            task3Segments, task3SegmentFunctions, task3Priorities,
                                            &task3Id);
  directive_failed(status, "Creation of segmented task 3 failed.");

  status = rtems_task_create(idleTaskName, 254, RTEMS_MINIMUM_STACK_SIZE, RTEMS_DEFAULT_MODES, RTEMS_DEFAULT_ATTRIBUTES, &idleTaskId);
  directive_failed(status, "Creation of idle task failed.");

  // Task Starting
  status = rtems_task_start_segmented_slfp(task1Id, (rtems_task_argument) taskArguments);
  directive_failed(status, "Start of task 1 failed.");

  status = rtems_task_start_segmented_slfp(task2Id, (rtems_task_argument) taskArguments);
  directive_failed(status, "Start of task 2 failed.");

  status = rtems_task_start_segmented_slfp(task3Id, (rtems_task_argument) taskArguments);
  directive_failed(status, "Start of task 3 failed.");

  status = rtems_task_start(idleTaskId, idle, (rtems_task_argument) NULL);
  directive_failed(status, "Start of idle Task failed.");

  puts("Exiting initialization Task");
  rtems_task_exit();
}