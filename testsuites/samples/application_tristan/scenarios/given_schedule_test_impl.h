#ifndef __TRISTAN_GIVEN_SCHEDULE_IMPL_TEST_H
#define __TRISTAN_GIVEN_SCHEDULE_IMPL_TEST_H

#include "../includations.h"

/********************************************
 *                                          *
 *              Given Schedule              *
 *                                          *
 ********************************************/

/**
 * - Three segmented tasks scheduled with slfp + one idle task
 * 
 * - First Task:
 *    - Period: 0
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
 *    - Period: 0
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
 *    - Period: 0
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

/********************************************
 *                                          *
 *              General Information         *
 *                                          *
 ********************************************/
static const size_t taskStackSize = RTEMS_MINIMUM_STACK_SIZE;
static const rtems_mode taskModes = RTEMS_DEFAULT_MODES;
static const rtems_attribute taskAttributes = RTEMS_DEFAULT_ATTRIBUTES;

static const rtems_name task1Name = rtems_build_name('T', '1', ' ', ' ');
static const rtems_name task2Name = rtems_build_name('T', '2', ' ', ' ');
static const rtems_name task3Name = rtems_build_name('T', '3', ' ', ' ');

uint32_t taskArguments[] = {10, 20, 30};

/********************************************
 *                                          *
 *                  Task 1                  *
 *                                          *
 ********************************************/
void task1segment1(Segmented_Task_Arguments args);
void task1segment2(Segmented_Task_Arguments args);
void task1segment3(Segmented_Task_Arguments args);

rtems_id task1Id;
rtems_id task1PeriodId;
rtems_interval task1Period = 0;
static const uint32_t task1Segments = 3;
rtems_task_priority task1Priorities[3] = {1, 1, 5};
static const uint32_t task1durations[3] = {3, 5, 2};
static const uint32_t task1ReleasedSegementsTotal = 1;
static const uint32_t task1ReleasedSegments[3] = {0, 1, 0};
/**
 * Syntax:
 * {Task which is to resume, When to resume it with respect to the resuming segments start time}
 */
static const uint32_t task1Releases[1][2] = {{task2Name, 2}};
static const void (*task1SegmentFunctions[]) (Segmented_Task_Arguments args) = {task1segment1, task1segment2, task1segment3};

/********************************************
 *                                          *
 *                  Task 2                  *
 *                                          *
 ********************************************/
void task2segment1(Segmented_Task_Arguments args);
void task2segment2(Segmented_Task_Arguments args);

rtems_id task2Id;
rtems_id task2PeriodId;
rtems_interval task2Period = 0;
static const uint32_t task2Segments = 2;
rtems_task_priority task2Priorities[2] = {2, 6};
static const uint32_t task2durations[2] = {1, 5};
static const uint32_t task2ReleasedSegementsTotal = 2;
static const uint32_t task2ReleasedSegments[2] = {0, 2};
static const uint32_t task2Releases[2][2] = {{task3Name, 4}, {task3Name, 5}}; // {Task which is to release, when to release}
static const void (*task2SegmentFunctions[]) (Segmented_Task_Arguments args) = {task2segment1, task2segment2};

/********************************************
 *                                          *
 *                  Task 3                  *
 *                                          *
 ********************************************/
void task3segment1(Segmented_Task_Arguments args);
void task3segment2(Segmented_Task_Arguments args);
void task3segment3(Segmented_Task_Arguments args);

rtems_id task3Id;
rtems_id task3PeriodId;
rtems_interval task3Period = 0;
static const uint32_t task3Segments = 3;
rtems_task_priority task3Priorities[3] = {3, 4, 7};
static const uint32_t task3durations[3] = {4, 3, 2};
static const uint32_t task3ReleasedSegementsTotal = 2;
static const uint32_t task3ReleasedSegments[3] = {1, 1, 0};
static const uint32_t task3Releases[2][2] = {{task1Name, 2}, {task1Name, 1}}; // {Task which is to release, when to release}
static const void (*task3SegmentFunctions[]) (Segmented_Task_Arguments args) = {task3segment1, task3segment2, task3segment3};

/********************************************
 *                                          *
 *                  Idle 3                  *
 *                                          *
 ********************************************/
rtems_name idleTaskName = rtems_build_name('I', 'd', 'l', 'e');

#endif