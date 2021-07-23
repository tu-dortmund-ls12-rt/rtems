#ifndef __TRISTAN_CUSTOM_SCENARIO_IMPL_H
#define __TRISTAN_CUSTOM_SCENARIO_IMPL_H

#include "../includations.h"

/********************************************
 *                                          *
 *                Scenario                  *
 *                                          *
 ********************************************/

/**
 * - One SLFP Task to validate periodicty.
 * - One idle task, to prevent system stallage.
 * 
 * - SLFP Task:
 *    - Period: 10s -> 10 ticks
 *    - 3 Segments
 *    - First Segment:
 *      - Priority: 1
 *      - Duration: 2s
 *      - Releases: Nothing
 *    - Second Segment:
 *      - Priority: 1
 *      - Duration: 2s
 *      - Releases: Nothing
 *    - Third Segment:
 *      - Priority: 1
 *      - Duration: 2s
 *      - Releases: Nothing
 * 
 * - Idle Task:
 *    - Period: 0
 *    - Non segmented task
 *    - Priority: 254
 *    - Releases: SLFP Task when possible.
 */

/********************************************
 *                                          *
 *              General Information         *
 *                                          *
 ********************************************/
static const size_t taskStackSize = RTEMS_MINIMUM_STACK_SIZE;
static const rtems_mode taskModes = RTEMS_DEFAULT_MODES;
static const rtems_attribute taskAttributes = RTEMS_DEFAULT_ATTRIBUTES;

static const rtems_name slfpTaskName = rtems_build_name('S', 'L', 'F', 'P');
static const rtems_name idleTaskName = rtems_build_name('I', 'D', 'L', 'E');

/********************************************
 *                                          *
 *                SLFP Task               *
 *                                          *
 ********************************************/
void slfpTaskSegment1(Segmented_Task_Arguments args);
void slfpTaskSegment2(Segmented_Task_Arguments args);
void slfpTaskSegment3(Segmented_Task_Arguments args);

rtems_id slfpTaskId;
rtems_id slfpTaskPeriodId;
rtems_interval slfpTaskPeriod = 10;
static const uint32_t slfpTaskSegments = 3;
rtems_task_priority slfpTaskPriorities[3] = {1, 1, 1};
static const void (*slfpTaskSegmentFunctions[]) (Segmented_Task_Arguments args) = {slfpTaskSegment1, slfpTaskSegment2, slfpTaskSegment3};

/********************************************
 *                                          *
 *                 Idle Task                *
 *                                          *
 ********************************************/
void idleTask(rtems_task_argument args);

rtems_id idleTaskId;
rtems_task_priority idleTaskPriority = 254;

#endif