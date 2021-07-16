#ifndef __TRISTAN_CUSTOM_SCENARIO_IMPL_H
#define __TRISTAN_CUSTOM_SCENARIO_IMPL_H

#include "../includations.h"

/********************************************
 *                                          *
 *                Scenario                  *
 *                                          *
 ********************************************/

/**
 * - One task "Memory Task" that to check the
 *   communication memory.
 * - One task "Buffer Task" to resume the 
 *   memory task.
 * - One idle task, to prevent system stallage.
 * 
 * - Memory Task:
 *    - 3 Segments
 *    - First Segment:
 *      - Priority: 1
 *      - Duration: Doesn't matter
 *      - Releases: Nothing
 *    - Second Segment:
 *      - Priority: 1
 *      - Duration: Doesn't matter
 *      - Releases: Nothing
 *    - Third Segment:
 *      - Priority: 1
 *      - Duration: Doesn't matter
 *      - Releases: Nothing
 * 
 * - Buffer Task:
 *    - Non segmented task
 *    - Priority: 2
 *    - Duration: 9s
 *    - Releases: Memory task at 3s, memory taskt at 6s
 * 
 * - Idle Task:
 *    - Non segmented task
 *    - Priority: 254
 *    - Releases: Nothing
 */

/********************************************
 *                                          *
 *              General Information         *
 *                                          *
 ********************************************/
static const size_t taskStackSize = RTEMS_MINIMUM_STACK_SIZE;
static const rtems_mode taskModes = RTEMS_DEFAULT_MODES;
static const rtems_attribute taskAttributes = RTEMS_DEFAULT_ATTRIBUTES;

static const rtems_name memoryTaskName = rtems_build_name('M', 'E', 'M', ' ');
static const rtems_name bufferTaskName = rtems_build_name('B', 'U', 'F', 'F');
static const rtems_name idleTaskName = rtems_build_name('I', 'D', 'L', 'E');

/********************************************
 *                                          *
 *                Memory Task               *
 *                                          *
 ********************************************/
void memoryTaskSegment1(Segmented_Task_Arguments args);
void memoryTaskSegment2(Segmented_Task_Arguments args);
void memoryTaskSegment3(Segmented_Task_Arguments args);

rtems_id memoryTaskId;
static const uint32_t memoryTaskSegments = 3;
rtems_task_priority memoryTaskPriorities[3] = {1, 1, 1};
static const void (*memoryTaskSegmentFunctions[]) (Segmented_Task_Arguments args) = {memoryTaskSegment1, memoryTaskSegment2, memoryTaskSegment3};

// Communication memory stuff
typedef struct {
  uint32_t value;
  uint8_t byte;
  char someName[256];
  void (*functionPtr) (void);
} CommunicationMemory;

void structFunction(void);

/********************************************
 *                                          *
 *                Buffer Task               *
 *                                          *
 ********************************************/
void bufferTask(rtems_task_argument args);

rtems_id bufferTaskId;
rtems_task_priority bufferTaskPriority = 2;
static const uint32_t bufferTaskDuration = 9;
static const uint32_t bufferTaskReleasedSegementsTotal = 2;
/**
 * Syntax:
 * {Task which is to resume, When to resume it with respect to the resuming segments start time}
 */
static const uint32_t bufferTaskReleases[2][2] = {{memoryTaskName, 3}, {memoryTaskName, 6}};

/********************************************
 *                                          *
 *                 Idle Task                *
 *                                          *
 ********************************************/
void idleTask(rtems_task_argument args);

rtems_id idleTaskId;
rtems_task_priority idleTaskPriority = 254;

#endif