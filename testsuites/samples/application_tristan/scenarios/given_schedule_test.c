#include "given_schedule_test.h"
#include "given_schedule_test_impl.h"

/********************************************
 *                                          *
 *         Task 1 - Segment Functions       *
 *                                          *
 ********************************************/
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

/********************************************
 *                                          *
 *         Task 2 - Segment Functions       *
 *                                          *
 ********************************************/
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

/********************************************
 *                                          *
 *         Task 3 - Segment Functions       *
 *                                          *
 ********************************************/
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

/********************************************
 *                                          *
 *              Idle Task                   *
 *                                          *
 ********************************************/
rtems_task idle(rtems_task_argument argument) {
  printf("Non-Idle Task finished. It's missing out rtems_task_exit() directive so the terminal doesn't freeze and the system halts.\n");
}

/********************************************
 *                                          *
 *           Execution Function             *
 *                                          *
 ********************************************/
void given_schedule_test_execute(void) {
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